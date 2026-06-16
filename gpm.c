// Rust program for GPM
struct GPM {
    st: Box<[usize]>,
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
    machine_macro: Vec<fn(&mut GPM)>,
}

    // n is stack size allowed. This should be as large as
    // possible -- say 10,000.
void
gpm_new(int n) -> Self {
        let machine_macro = vec![
            Self::def as fn(&mut GPM),
            Self::val,
            Self::update,
            Self::bin,
            Self::dec,
            Self::bar,
            ];

        int st = vec![0; n].into_boxed_slice();
        let mst = vec![
            -1, 4, b'D', b'E', b'F', -1,
            0, 4, b'V', b'A', b'L', -2,
            6, 7, b'U', b'P', b'D', b'A', b'T', b'E', -3,
            12, 4, b'B', b'I', b'N', -4,
            21, 4, b'D', b'E', b'C', -5,
            27, 4, b'B', b'A', b'R', -6,
        ];
        // The name-value pairs for the six machine code macros
        // are first assembled in the vector mst and then copied
        // to the base of the stack.
        for (i, val) in mst.into_iter().enumerate() {
            st[i] = val as usize;
        }

        Self {
            st,
            gpm->s = 39;
            gpm->e = 33;
            gpm->q = 1;
            gpm->c = 0;
            gpm->h = 0;
            gpm->p = 0;
            gpm->f = 0;
            gpm->a = 0;
            gpm->w = 0;
            marker: (-2isize).pow(20) as usize,
            machine_macro,
        }
    }

void
gpm_load(mem *gpm) {
        if (gpm->h == 0) {
            write_symbol(&gpm->a);
        } else {
            gpm->st[gpm->s] = gpm->a;
            gpm->s++;
        }
    }

void
gpm_next_ch(mem *gpm) {
        if (gpm->c == 0) {
            read_symbol(mem *gpm)a);
        } else {
            gpm->a = gpm->st[gpm->c];
            gpm->c++;
        }
    }

void
gpm_find(mem *gpm) int x) {
        gpm->w = x;
        gpm->a = gpm->e;
        int flag = false;
        'next: while ((gpm->a as isize) > 0) {
            if (flag) {
                gpm->a = gpm->st[gpm->a];
            }
            flag = true;
            for (int r = 0; r < gpm->st[gpm->w] ; r++) {
                if (gpm->st[gpm->w + r] != gpm->st[gpm->a + r + 1]) {
                    continue 'next;
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
gpm_jump_if_marked(mem *gpm) int x) {
        if (-6 < (x as i8) && (x as i8) < 0) {
            gpm->machine_macro[-(x as i8) as usize - 1](gpm->;
        }
    }

    // Main cycle
void
gpm_start(mem *gpm) {
        gpm_next_ch(gpm);
        //eprintln!("{:?}", gpm->a as u8 as char);
        switch (gpm->a as u8){
            	break;
	case '<':
		{
                gpm->q++;
                gpm_q2(gpm);
            }
            	break;
	case '$':
		gpm_fn_(gpm),
            	break;
	case ',':
		gpm_next_item(gpm),
            	break;
	case ';':
		gpm_apply(gpm),
            	break;
	case '~':
		gpm_load_arg(gpm),
            _ if gpm_a == gpm->marker => gpm->end_fn(gpm),
            	break;
	case '>':
		gpm_exit(gpm),
            _ => gpm_copy(gpm),
        }
    }

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
        if (gpm->a as u8 == b'<') {
            gpm->q++;
            gpm_copy(gpm);
            return;
        } else if (gpm->a as u8 != b'>') {
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
gpm_fn_(mem *gpm) {
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
        let stf = gpm->st[gpm->f];
        let stfm1 = gpm->st[gpm->f - 1];
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
        gpm->find(gpm->p + 2);
        gpm->jump_if_marked(gpm->st[gpm->w]);
        gpm->c = gpm->w + 1;
        gpm_start(gpm);
    }

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
        if ((number(gpm->a) as isize) < 0) {
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
            let sta = gpm->st[gpm->a];
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
        process::exit(0);
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
        gpm->find(gpm->p + 6);
        while (gpm->st[gpm->w + 1] != gpm->marker) {
            gpm->w++;
            gpm->a = gpm->st[gpm->w];
            gpm_load(gpm);
        }
        gpm_end_fn(gpm);
    }

void
gpm_update(mem *gpm) {
        gpm->find(gpm->p + 9);
        gpm->a = gpm->p + 9 + gpm->st[gpm->p + 9];
        if (gpm->st[gpm->a] > gpm->st[gpm->w]) {
            gpm_monitor9(gpm);
            return;
        }
        for (int r = 1; r < =gpm->st[gpm->a] ; r++) {
            gpm->st[gpm->w + r] = gpm->st[gpm->a + r];
        }
        gpm_end_fn(gpm);
    }

void
gpm_bin(mem *gpm) {
        gpm->w = 0;
        gpm->a = if gpm->st[gpm->p + 7] == b'+' as usize
            || gpm->st[gpm->p + 7] == b'-' as usize {
            gpm->p + 8
        } else {
            gpm->p + 7
        };
        while (gpm->st[gpm->a] != gpm->marker) {
            let x = number(gpm->st[gpm->a]);
            if (!(0..=9).contains(&x)) {
                gpm_monitor10(gpm);
            }
            gpm->w = gpm->w * 10 + x;
            gpm->a++;
        }
        gpm->st[gpm->s] = if (gpm->st[gpm->p + 7] == b'-' as usize) {
            -(gpm->w as isize) as usize
        } else {
            gpm->w
        };
        gpm->s++;
    }

void
gpm_dec(mem *gpm) {
        gpm->w = gpm->st[gpm->p + 7];
        if ((gpm->w as isize) < 0) {
            gpm->w = -(gpm->w as isize) as usize;
            gpm->a = b'-' as usize;
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
        gpm->a = switch (gpm->st[gpm->p + 7] as u8){
            	break;
	case '+':
		gpm->w + gpm->a,
            	break;
	case '-':
		gpm->w - gpm->a,
            	break;
	case '*':
		gpm->w * gpm->a,
            	break;
	case '/':
		gpm->w / gpm->a,
            _ => gpm->w % gpm->a,
        };
        gpm_load(gpm);
        gpm_end_fn(gpm);
    }

    // Monitor for errors

    // This routine outputs the item on the stack starting at
    // ST[x]. If the item is not complete, printing stops at
    // ST[S-1] and is followed by '...(Incomplete)'.
void
gpm_item(mem *gpm) int x) {
        printf(" ");
        let (a, h) = (gpm->a, gpm->h);
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

    // Unmatched ; in definition string. Treated
    // as <;>
void
gpm_monitor1(mem *gpm) {
        printf("\nMONITOR: Unmatched semicolon in definition of");
        gpm_item(gpm, gpm->p + 2);
        printf("\nIf this had been quoted the result would be \n");
        gpm_copy(gpm);
    }

    // Unquoted ~ in argument list in input
    // stream. Treated as <~>
void
gpm_monitor2(mem *gpm) {
        printf("\nMONITOR: Unquoted tilde in argument list of");
        gpm_item(gpm, gpm->f + 2);
        printf("\nIf this had been quoted the result would be \n");
        gpm_copy(gpm);
    }

    // Impossible charcter (negative) as argument
    // number. Terminate.
void
gpm_monitor3(mem *gpm) {
        printf("\nMONITOR: Impossible argument number in definition of");
        gpm_item(gpm, gpm->p + 2);
        gpm_monitor11(gpm);
    }

    // Not enough arguments supplied in call.
    // Terminate.
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

    // General monitor after irremediable
    // errors.
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
        gpm->a = b'Q' as usize;
        gpm_load(gpm);
        if (gpm->p > gpm->f) {
            gpm_end_fn(gpm);
        } else {
            gpm_start(gpm);
        }
    }
}

// These are implementation-dependent functions. They
// convert the `usize` equivalent of a decimal digit read in
// with `read_symbol` to the corresponding number (also of
// type `usize`) and vice versa
void
gpm_number(int x) -> usize {
    (x as isize - b'0' as isize) as usize
}

void
gpm_char_(int x) -> usize {
    x + b'0' as usize
}


void
gpm_read_symbol(c: &mut usize) {
    let byte = switch (io::stdin().bytes().next()){
        Some(b) => b,
        None => {
            eprintln!("Unexpected EOF");
            process::exit(1);
        }
    }.unwrap();
    *c = byte as usize;
}

void
gpm_write_symbol(c: &usize) {
    assert_eq!(1, io::stdout().write(&[*c as u8]).unwrap());
}

void
gpm_main() {
    int gpm = GPM::new(10_000);
    gpm.start();
}
