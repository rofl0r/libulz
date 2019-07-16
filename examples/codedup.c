#include "../include/tglist.h"
#include "../include/hbmap.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <assert.h>

static unsigned string_hash(const char* s) {
	uint_fast32_t h = 0;
	while (*s) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
	}
	return h & 0xfffffff;
}

static int line_equal(unsigned hash1, char* s1, unsigned hash2, char* s2) {
	return hash1 == hash2 && !strcmp(s1, s2);
}

static int intcmp(const void*c1, const void*c2) {
	const int* i1 = c1;
	const int* i2 = c2;
	return *i1 - *i2;
}

static int usage(char *a0) {
	dprintf(2,
		"%s - duplicate text/code finder\n"
		"finds the biggest duplicated blocks\n"
		"usage %s [-w] [-i min] [-s start] [-e end] file\n"
		"-w: remove leading whitespace\n"
		"-i min: specify minimal block length in lines (default: 4)\n"
		"-s start: specify start line (0-based index)\n"
		"-e end: specify end line (0-based index)\n"
		"-a atime: number of seconds before updating status (0: no status)\n"
		"file may be specified as '-' in which case stdin is used\n"
		"output goes to stdout.\n"
		, a0, a0);
	return 1;
}

static unsigned inthash(int i) {return i;}

static int sigc, neednl;
static int cl, nl, stdout_is_tty, atime = 1;
static void sigh(int nsig) {
	sigc++;
	dprintf(2, "\rprocessed %d/%d (%.2f%%), elapsed %d", cl, nl, (cl/(float)nl)*100.f, sigc*atime);
	alarm(atime);
	neednl = 1;
}

static int sortitem_cmp(const void* a, const void* b) {
	const struct { int dupes; int lineno; } *a1 =a, *b1 = b;
	int ret = b1->dupes - a1->dupes;
	if(ret == 0) return a1->lineno - b1->lineno;
	return ret;
}

int main(int argc, char **argv) {
	int c, whitespace_flag = 0;
	size_t blocklen_min = 4, start = 0, end = -1;
	while((c = getopt(argc, argv, "wi:s:e:a:")) != -1) switch(c) {
		case 'w': whitespace_flag = 1; break;
		case 'i': blocklen_min = atoi(optarg); break;
		case 's': start = atoi(optarg); break;
		case 'e': end = atoi(optarg); break;
		case 'a': atime = atoi(optarg); break;
		default: return usage(argv[0]);
	}
	if(!argv[optind]) return usage(argv[0]);
	FILE *f = argv[optind][0] == '-' && !argv[optind][1] ? stdin : fopen(argv[optind], "r");
	if(!f) {
		perror("fopen");
		return 1;
	}
	stdout_is_tty = isatty(fileno(stdout));
	struct sortitem { int dupes; int lineno; };
	tglist(unsigned) hashlist;
	tglist(char*) strlist;
	typedef tglist(struct sortitem) sortitem_list; /* we need a named type to prevent pointer type mismatch warnings */
	hbmap(int, sortitem_list*, 1024) dupes;
	tglist_init(&hashlist);
	tglist_init(&strlist);
	hbmap_init(&dupes, intcmp, inthash);
	char buf[1024];
	size_t i = 0;
	while(fgets(buf, sizeof buf, f)) {
		++i;
		if(i - 1 < start) continue;
		if(end != (size_t) -1 && i >= end) break;
		char *p = buf, *q = strrchr(p, '\n');
		if(q) *q = 0;
		if(whitespace_flag) while(isspace(*p)) p++;
		unsigned hash = string_hash(p);
		tglist_add(&hashlist, hash);
		tglist_add(&strlist, strdup(p));
		assert(tglist_getsize(&strlist) == i - start);
		assert(tglist_getsize(&strlist) == tglist_getsize(&hashlist));
		assert(!strcmp(p, tglist_get(&strlist, i-start-1)));
	}
	size_t linecount = tglist_getsize(&hashlist);
	size_t bl;
	nl = linecount;

	if(atime) {
		signal(SIGALRM, sigh);
		alarm(atime);
	}

	//for(bl = blocklen_max; bl >= blocklen_min; bl--) {
	for(bl = blocklen_min; bl < blocklen_min+1; bl++) {
		for(i = 0; i < linecount; ) {
			hbmap_iter hi;
			int dupe_count = 0, longest_dupe = 0;
			size_t j, h, last_dupe = 0;
			for(j = i + bl; j + bl < linecount; ) {
#define HL(X) tglist_get(&hashlist, X)
#define SL(X) tglist_get(&strlist, X)
				for(h = 0; h < bl; ++h) {
					if(!line_equal(HL(i+h), SL(i+h), HL(j+h), SL(j+h)))
						break;
				}
				if(h == bl) {
					/* look if the dupe is actually longer */
					while(j+h < linecount && i+h < j && line_equal(HL(i+h), SL(i+h), HL(j+h), SL(j+h)))
						h++;
					sortitem_list* dlist = 0;
					hi = hbmap_find(&dupes, i);

					if(hi == (hbmap_iter) -1)
						dlist = tglist_new();
					else
						dlist = hbmap_getval(&dupes, hi);

					struct sortitem tmp = {.dupes = h, .lineno = j};
					tglist_insert_sorted(dlist, tmp, sortitem_cmp);

					if(hi == (hbmap_iter) -1)
						hbmap_insert(&dupes, i, dlist);

					last_dupe = j;
					if(h > longest_dupe) longest_dupe = h;
					++dupe_count;

				}
				j+= h ? h : 1;
			}
			i += dupe_count ? longest_dupe : 1;
			cl = i;
		}
	}
	hbmap_iter hi;
	sortitem_list si_list;
	tglist_init(&si_list);
	hbmap_foreach(&dupes, hi) {
		int longest = 0;
		sortitem_list*dlist = hbmap_getval(&dupes, hi);
		tglist_foreach(dlist, i) {
			struct sortitem *si = &tglist_get(dlist, i);
			if(si->dupes > longest) longest = si->dupes;
		}
		assert(longest == tglist_get(dlist, 0).dupes);
		struct sortitem si = {.dupes = longest, .lineno = hbmap_getkey(&dupes, hi)};
		tglist_insert_sorted(&si_list, si, sortitem_cmp);
	}
	for(i = 0; i < tglist_getsize(&si_list); i++) {
		struct sortitem *si = &tglist_get(&si_list, i);
		hi = hbmap_find(&dupes, si->lineno);
		sortitem_list *dlist = hbmap_getval(&dupes, hi);
		int j, cnt = 0;
		tglist_foreach(dlist, j) {
			if(tglist_get(dlist, j).dupes < si->dupes) break;
			++cnt;
		}
		if(neednl && stdout_is_tty) {
			dprintf(2, "\n");
			neednl = 0;
		}
		printf("[DUP] %d lines at %d: %d dupes (", si->dupes, (int) start + si->lineno, cnt);
		for(j = 0; j < cnt; ++j)
			printf("%d%s", (int) start + tglist_get(dlist, j).lineno, j == cnt -1 ? ")\n" : ", ");
		for(j = 0; j < si->dupes; j++)
			printf("%s\n", tglist_get(&strlist, si->lineno+j));
	}
	if(f != stdin) fclose(f);
	if(neednl) dprintf(2, "\n");
	return 0;
}
