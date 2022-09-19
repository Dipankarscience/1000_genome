#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define SIZE 100000

int main()
{
    FILE *fp, *fp1;

    fp = fopen("ALL.chrY.phase3_integrated_v1a.20130502.genotypes.vcf","r");
    fp1 = fopen("FILTERED_DATA_SET.txt", "w");

    int flag = 0;
    char ch1;

    while(1)
    {
        char ch = fgetc(fp);
        if(ch == EOF)
            break;
        else if (ch == '#' && (ch1 = fgetc(fp)) == '#')
            while (1) {
                char ch1 = fgetc(fp);
                if (ch1 == '\n')
                    break;
                else
                    continue;
            }
        else {
            fputc(ch, fp1);
            if (flag == 0)
            {
                fputc(ch1, fp1);
                flag = 1;
            }
        }
    }

    fclose(fp1);

    FILE *fp2 = fopen("FILTERED_DATA_SET.txt", "r");

    char data[SIZE];

    char ch3 = fgetc (fp2);

    data[0] = ch3;
    int i = 1;

    while (ch3 != '\n')
    {
        ch3 = fgetc (fp2);
        data [i++] = ch3;
    }

    data [i] = '\0';
    printf ("%s\n", data);

    long unsigned idx = 0;
    long unsigned col = 0;

    while (data[idx] != '\0') {
        idx++;
        if (data[idx] == '\t') {
            col++;
        }
    }
    col++;
    printf("No.of Attributes : %lu\n", col);

    long unsigned total_no_rows = 0;
    long unsigned no_of_filtered_rows = 0;
    bool redundant_row;

    while(fp2 != NULL) {
        fgets(data, SIZE, fp2);

        redundant_row = false;
        total_no_rows++;
        idx = 0;
        int col_idx = 0;

        while (data[idx] != '\0') {
            char w[200];
            int w_idx = 0;

            for (idx; data[idx] != '\t' && data[idx] != '\0'; idx++){
                w[w_idx++] = data[idx];
            }

            w[w_idx]='\0';

            if (col_idx == 3) {
                if (strlen(w)!=1 || !(w[0]=='A' || w[0]=='T' || w[0]=='G' || w[0]=='C')) {
                    redundant_row = true;
                    break;
                }
            }

            if (col_idx == 4) {
                int ALT_att_size = strlen(w);

                if (ALT_att_size % 2 == 0) {
                    redundant_row = true;
                } else {
                    for (int i = 0; i < ALT_att_size; i++) {
                        if (i % 2 == 0 && !(w[i]=='A' || w[i]=='T' || w[i]=='G'|| w[i]=='C')) {
                            redundant_row = true;
                            break;
                        } else if (i % 2 != 0 && w[i]!=',') {
                            redundant_row = true;
                            break;
                        }
                    }
                }

                if (redundant_row) {
                    break;
                }
            }

            if (col_idx == 6 && strcmp(w,"PASS") != 0) {
                redundant_row = true;
                break;
            }

            col_idx++;

            if (data[idx] == '\t') {
                idx++;
            }
        }

        if (!redundant_row) {
            no_of_filtered_rows++;
        }
    }

    printf("Total number of rows: %lu\n", total_no_rows);
    printf("Number of filtered rows: %lu\n", no_of_filtered_rows);

    fclose(fp);
    fclose(fp2);

    return 0;
}
