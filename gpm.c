// C implementation of Christopher Strachey's GPM

#include <stdio.h>
#include <stdlib.h>

typedef struct mem mem;
typedef void (*fn)(mem *);

struct mem {
    int s;
    int e;
    int q;
    int c;
    int h;
    int p;
    int f;
    int a;
    int w;
    int marker;
	fn prims[6];
        int st[];
};

#define write_symbol putchar

void
gpm_load(mem *gpm) {
        if (gpm->h == 0) {
            write_symbol(gpm->a);
        } else {
            gpm->st[gpm->s] = gpm->a;
            gpm->s++;
        }
    }

void
read_symbol(int *c) {
        int ch = getchar();
        *c= (ch == EOF) ? '>' : ch;
}

void
gpm_next_ch(mem *gpm) {
        if (gpm->c == 0) {
            read_symbol(&gpm->a);
        } else {
            gpm->a = gpm->st[gpm->c];
            gpm->c++;
        }
    }

void gpm_monitor1(mem *);
void gpm_monitor2(mem *);
void gpm_monitor3(mem *);
void gpm_monitor4(mem *);
void gpm_monitor5(mem *);
void gpm_monitor6(mem *);
void gpm_monitor7(mem *);
void gpm_monitor8(mem *);
void gpm_monitor9(mem *);
void gpm_monitor10(mem *);
void gpm_monitor11(mem *);

void
gpm_find(mem *gpm, int x) {
        gpm->w = x;
        gpm->a = gpm->e;
        int flag = false;
next:	while (gpm->a > 0) {
            if (flag) {
                gpm->a = gpm->st[gpm->a];
            }
            flag = true;
            for (int r = 0; r < gpm->st[gpm->w] ; r++) {
                if (gpm->st[gpm->w + r] != gpm->st[gpm->a + r + 1]) {
                    goto next;
                }
            }
            gpm->w = gpm->a + 1 + gpm->st[gpm->w];
            return;
        }
        gpm_monitor7(gpm);
    }

// This routine depends on the method for marking machine
// code macros. The method adopted here (which is
// different from that described in the paper or used in the
// actual Titan program) is to make the value a negative
// index integer which is used to index the label vector
// machine_macro, whose entries are the labels of the
// corresponding programs.
void
gpm_jump_if_marked(mem *gpm, char x) {
        if (-6 < x && x < 0) {
            gpm->prims[-x - 1](gpm);
        }
    }

void gpm_q2(mem *);
void gpm_start(mem *);

void
gpm_copy(mem *gpm) {
        gpm_load(gpm);
        if (gpm->q == 1) {
            gpm_start(gpm);
        } else {
            gpm_q2(gpm);
        }
    }

void
gpm_q2(mem *gpm) {
        gpm_next_ch(gpm);
        if (gpm->a == '<') {
            gpm->q++;
            gpm_copy(gpm);
            return;
        } else if (gpm->a != '>') {
            gpm_copy(gpm);
            return;
        }
        gpm->q -= 1;
        if (gpm->q == 1) {
            gpm_start(gpm);
        } else {
            gpm_copy(gpm);
        }
    }

// Warning character actions

void
gpm_macro(mem *gpm) {
        gpm->st[gpm->s] = gpm->h;
        gpm->st[gpm->s + 1] = gpm->f;
        gpm->st[gpm->s + 2] = 0;
        gpm->st[gpm->s + 3] = 0;
        gpm->h = gpm->s + 3;
        gpm->f = gpm->s + 1;
        gpm->s += 4;
        gpm_start(gpm);
    }

void
gpm_next_item(mem *gpm) {
        if (gpm->h == 0) {
            gpm_copy(gpm);
            return;
        }
        gpm->st[gpm->s] = 0;
        gpm->st[gpm->h] = gpm->s - gpm->h - gpm->st[gpm->h];
        gpm->h = gpm->s;
        gpm->s++;
        gpm_start(gpm);
    }

void
gpm_apply(mem *gpm) {
        if (gpm->p > gpm->f) {
            gpm_monitor1(gpm);
            return;
        } else if (gpm->h == 0) {
            gpm_copy(gpm);
            return;
        }
        int stf = gpm->st[gpm->f];
        int stfm1 = gpm->st[gpm->f - 1];
        gpm->st[gpm->f + 1] = gpm->c;
        gpm->st[gpm->f] = gpm->p;
        gpm->st[gpm->f - 1] = gpm->s - gpm->f + 2;
        gpm->st[gpm->s] = gpm->marker;
        gpm->st[gpm->h] = gpm->s - gpm->h;
        gpm->s++;
        gpm->h = stfm1;
        gpm->p = gpm->f;
        gpm->f = stf;
        if (gpm->h != 0) {
            gpm->st[gpm->h] += gpm->st[gpm->p - 1];
        }
        gpm_find(gpm, gpm->p + 2);
        gpm_jump_if_marked(gpm, gpm->st[gpm->w]);
        gpm->c = gpm->w + 1;
        gpm_start(gpm);
    }

#define number(x) ((x)-'0')

void
gpm_load_arg(mem *gpm) {
        if (gpm->p == 0) {
            if (gpm->h == 0) {
                gpm_copy(gpm);
                return;
            } else {
                gpm_monitor2(gpm);
                return;
            }
        }
        gpm_next_ch(gpm);
        gpm->w = gpm->p + 2;
        if (number(gpm->a) < 0) {
            gpm_monitor3(gpm);
            return;
        }
        for (int _ = 0; _ < number(gpm->a) ; _++) {
            gpm->w += gpm->st[gpm->w];
            if (gpm->st[gpm->w] == gpm->marker) {
                gpm_monitor4(gpm);
                return;
            }
        }
        for (int r = 1; r < gpm->st[gpm->w] ; r++) {
            gpm->a = gpm->st[gpm->w + r];
            gpm_load(gpm);
        }
        gpm_start(gpm);
    }

void
gpm_end_fn(mem *gpm) {
        if (gpm->f > gpm->p) {
            gpm_monitor5(gpm);
            return;
        }

        gpm->a = gpm->s;
        gpm->st[gpm->s] = gpm->e;

        while (gpm->st[gpm->a] >= gpm->p - 1 + gpm->st[gpm->p - 1]) {
            int sta = gpm->st[gpm->a];
            gpm->st[gpm->a] -= gpm->st[gpm->p - 1];
            gpm->a = sta;
        }

        gpm->w = gpm->st[gpm->a];

        while (gpm->w >= gpm->p - 1) {
            gpm->w = gpm->st[gpm->w];
        }

        gpm->st[gpm->a] = gpm->w;
        gpm->e = gpm->st[gpm->s];

        if (gpm->h != 0) {
            if (gpm->h > gpm->p) {
                gpm->h -= gpm->st[gpm->p - 1];
            } else {
                gpm->st[gpm->h] -= gpm->st[gpm->p - 1];
            }
        }

        gpm->w = gpm->p - 1 + gpm->st[gpm->p - 1];
        gpm->a = gpm->p - 1;
        gpm->s -= gpm->st[gpm->p - 1];
        gpm->c = gpm->st[gpm->p + 1];
        gpm->p = gpm->st[gpm->p];

        while (gpm->a != gpm->s) {
            gpm->st[gpm->a] = gpm->st[gpm->w];
            gpm->w++;
            gpm->a++;
        }
        gpm_start(gpm);
    }


void
gpm_exit(mem *gpm) {
        if (gpm->c != 0 || gpm->h != 0) {
            gpm_monitor8(gpm);
            return;
        }
        exit(0);
    }

// Main cycle
void
gpm_start(mem *gpm) {
	gpm_next_ch(gpm);

	switch (gpm->a){
		case '<':
			gpm->q++;
			gpm_q2(gpm);
			break;
		case '$':
			gpm_macro(gpm);
			break;
		case ',':
			gpm_next_item(gpm);
			break;
		case ';':
			gpm_apply(gpm);
			break;
		case '~':
			gpm_load_arg(gpm);
			break;
		case '>':
			gpm_exit(gpm);
			break;
		default :
			(gpm->a == gpm->marker) ? gpm_end_fn(gpm) : gpm_copy(gpm);
		}
	}

// Machine code macros

// This version of DEF is shorter than that given in
// Section 7 as it leaves `end_fn` to copy back the definition
void
gpm_def(mem *gpm) {
        if (gpm->h != 0) {
            gpm->st[gpm->h] -= gpm->st[gpm->p - 1] - 6;
        }
        gpm->st[gpm->p - 1] = 6;
        gpm->st[gpm->p + 5] = gpm->e;
        gpm->e = gpm->p + 5;
        gpm_end_fn(gpm);
    }

void
gpm_val(mem *gpm) {
        gpm_find(gpm, gpm->p + 6);
        while (gpm->st[gpm->w + 1] != gpm->marker) {
            gpm->w++;
            gpm->a = gpm->st[gpm->w];
            gpm_load(gpm);
        }
        gpm_end_fn(gpm);
    }

void
gpm_update(mem *gpm) {
        gpm_find(gpm, gpm->p + 9);
        gpm->a = gpm->p + 9 + gpm->st[gpm->p + 9];
        if (gpm->st[gpm->a] > gpm->st[gpm->w]) {
            gpm_monitor9(gpm);
            return;
        }
        for (int r = 1; r <= gpm->st[gpm->a] ; r++) {
            gpm->st[gpm->w + r] = gpm->st[gpm->a + r];
        }
        gpm_end_fn(gpm);
    }

void
gpm_bin(mem *gpm) {
        gpm->w = 0;
        gpm->a =  ((gpm->st[gpm->p + 7] == '+') || (gpm->st[gpm->p + 7] == '-')) ? gpm->p + 8 :  gpm->p + 7;
        while (gpm->st[gpm->a] != gpm->marker) {
            int x = number(gpm->st[gpm->a]);
            if (x > 9) {
                gpm_monitor10(gpm);
            }
            gpm->w = gpm->w * 10 + x;
            gpm->a++;
        }
        gpm->st[gpm->s] = (gpm->st[gpm->p + 7] == '-') ? -(gpm->w) : gpm->w;
        gpm->s++;
    }

#define char_(x) ((x)+'0')

void
gpm_dec(mem *gpm) {
        gpm->w = gpm->st[gpm->p + 7];
        if (gpm->w < 0) {
            gpm->w = -gpm->w;
            gpm->a = '-';
            gpm_load(gpm);
        }
        int w1 = 1;
        while (10 * w1 <= gpm->w) {
            w1 *= 10;
        }
        for (;;) {
            gpm->a = char_(gpm->w / w1);
            gpm->w %= w1;
            w1 /= 10;
            if (w1 < 1) {
                break;
            }
        }
        gpm_end_fn(gpm);
    }

void
gpm_bar(mem *gpm) {
        gpm->w = gpm->st[gpm->p + 9];
        gpm->a = gpm->st[gpm->p + 11];
        switch (gpm->st[gpm->p + 7]){
		case '+': gpm->a = gpm->w + gpm->a; break;
		case '-': gpm->a = gpm->w - gpm->a; break;
		case '*': gpm->a = gpm->w * gpm->a; break;
		case '/': gpm->a = gpm->w / gpm->a; break;
		default : gpm->a = gpm->w % gpm->a; break;
        };
        gpm_load(gpm);
        gpm_end_fn(gpm);
    }

// Monitor for errors

// This routine outputs the item on the stack starting at
// ST[x]. If the item is not complete, printing stops at
// ST[S-1] and is followed by '...(Incomplete)'.
void
gpm_item(mem *gpm, int x) {
        printf(" ");
        int a = gpm->a;
	int h = gpm->h;
        gpm->h = 0;
        int k = 1;
        for (;;) {
            if (gpm->st[x] == 0) {
                if (k == gpm->s - x) {
                    break;
                }
            } else {
                if (k == gpm->st[x]) {
                    break;
                }
            }
            gpm->a = gpm->st[x + k];
            gpm_load(gpm);
            k++;
        }
        if (gpm->st[x] == 0) {
            printf("...\t(Incomplete)");
        }
        gpm->a = a;
        gpm->h = h;
    }

// Monitor entries and effects

// Unmatched } in definition string. Treated as [}]
void
gpm_monitor1(mem *gpm) {
        printf("\nMONITOR: Unmatched semicolon in definition of");
        gpm_item(gpm, gpm->p + 2);
        printf("\nIf this had been quoted the result would be \n");
        gpm_copy(gpm);
    }

// Unquoted $ in argument list in input stream. Treated as [$]
void
gpm_monitor2(mem *gpm) {
        printf("\nMONITOR: Unquoted tilde in argument list of");
        gpm_item(gpm, gpm->f + 2);
        printf("\nIf this had been quoted the result would be \n");
        gpm_copy(gpm);
    }

// Impossible character (negative) as argument number. Terminate.
void
gpm_monitor3(mem *gpm) {
        printf("\nMONITOR: Impossible argument number in definition of");
        gpm_item(gpm, gpm->p + 2);
        gpm_monitor11(gpm);
    }

// Not enough arguments supplied in call. Terminate.
void
gpm_monitor4(mem *gpm) {
        printf("\nMONITOR: No argument ");
        gpm->h = 0;
        gpm_load(gpm);
        printf(" in call for");
        gpm_item(gpm, gpm->p + 2);
        gpm_monitor11(gpm);
    }

// Terminator in impossible place; if C == 0,
// this is the input stream. Probably
// machine error: Terminate. If C != 0, this
// is an argument list. Probably due to a
// missing semicolon: Final semicolon inserted.
void
gpm_monitor5(mem *gpm) {
        printf("\nMONITOR: Terminator in");
        if (gpm->c == 0) {
            printf("input stream. Probably machine error.");
            gpm_monitor11(gpm);
            return;
        }
        printf("argument list for");
        gpm_item(gpm, gpm->f + 2);
        printf("\nProbably due to a semicolon missing from the definition of");
        gpm_item(gpm, gpm->p + 2);
        printf("\nIf a final semicolon is inserted the result is \n");
        gpm->c -= 1;
        gpm_apply(gpm);
    }

// Undefined macro name: Terminate.
void
gpm_monitor7(mem *gpm) {
        printf("\nMONITOR: Undefined name");
        gpm_item(gpm, gpm->w);
        gpm_monitor11(gpm);
    }

// Wrong exit (not C == H == 0). Machine
// error: Terminate.
void
gpm_monitor8(mem *gpm) {
        printf("\nMONITOR: Unmatched >. Probably machine error.");
        gpm_monitor11(gpm);
    }

// Update string too long: Terminate.
void
gpm_monitor9(mem *gpm) {
        printf("\nMONITOR: Update argument too long for");
        gpm_item(gpm, gpm->p + 9);
        gpm_monitor11(gpm);
    }

// Non-digit in argument for BIN. Terminate.
void
gpm_monitor10(mem *gpm) {
        printf("\nMONITOR: Non-digit in number");
        gpm_monitor11(gpm);
    }

// General monitor after irremediable errors.
void
gpm_monitor11(mem *gpm) {
        gpm->w = 20;
        printf("\nCurrent macros are");
        while (gpm->p != 0 || gpm->f != 0) {
            int w1;
            if (gpm->p > gpm->f) {
                w1 = gpm->p + 2;
                gpm->p = gpm->st[gpm->p];
                printf("\nAlready entered");
            } else {
                w1 = gpm->f + 2;
                gpm->f = gpm->st[gpm->f];
                printf("\nNot yet entered");
            }
            for (int r = 1; r < gpm->w ; r++) {
                gpm_item(gpm, w1);
                if (gpm->st[w1] == 0) {
                    break;
                }
                w1 += gpm->st[w1];
                if (gpm->st[w1] == gpm->marker) {
                    break;
                }
                if (gpm->w != 1) {
                    printf("\nArg {}\t", r);
                }
            }
            gpm->w = 1;
        }
        printf("\nEnd of monitor printing");
        gpm->a = 'Q';
        gpm_load(gpm);
        if (gpm->p > gpm->f) {
            gpm_end_fn(gpm);
        } else {
            gpm_start(gpm);
        }
    }


// n is stack size allowed. This should be as large as
// possible -- say 10,000.
mem *
gpm_new(int n) {
	mem *gpm = malloc(n);
	if (!gpm) {
                fprintf(stderr, "Out of memory\n");
                exit(1);
        }

	fn prims[] = {gpm_def, gpm_val, gpm_update, gpm_bin, gpm_dec, gpm_bar};
        for (int i = 0; i < sizeof(prims) / sizeof(prims[0]); i++) {
                gpm->prims[i]=prims[i];
        }

        unsigned char mst[] = {
           -1, 4, 'D', 'E', 'F', -1,
            0, 4, 'V', 'A', 'L', -2,
            6, 7, 'U', 'P', 'D', 'A', 'T', 'E', -3,
            12, 4, 'B', 'I', 'N', -4,
            21, 4, 'D', 'E', 'C', -5,
            27, 4, 'B', 'A', 'R', -6,
        };

        // The name-value pairs for the six machine code macros
        // are first assembled in the vector mst and then copied
        // to the base of the stack.
	for (int i = 0; i < sizeof(mst) / sizeof(mst[0]); i++) {
                gpm->st[i] = mst[i];
	}
	gpm->s = 39;
	gpm->e = 33;
	gpm->q = 1;
	gpm->c = 0;
	gpm->h = 0;
	gpm->p = 0;
	gpm->f = 0;
	gpm->a = 0;
	gpm->w = 0;
	gpm->marker = -1<<20;
	return gpm;
    }

void
main() {
    mem *gpm = gpm_new(10000);
    gpm_start(gpm);
}
