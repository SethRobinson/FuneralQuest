#pragma once

bool compare(char *orig, char *comp);
void getdir(char *dir, char *final);
#define RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }
#define ODS(a) OutputDebugString(a)
void Msg( LPSTR fmt, ... );
void switch_to_current_dir();
bool exist(char name[255]);
char *show_date(void);
char *show_time(void);
bool in_range(int i_num, int i_low, int i_high);
void add_text(char *tex ,char *filename);
FILE * open_file(char file[255]);
bool get_line_from_file(FILE * fp, char * dest);
void replace(const char *this1, char that[255], char *line);
char *show_small_date(void);
char *show_army_time(void);
char *show_date_month_and_day(void);
int random(long i_max);
int random(long i_max, long i_min);
int random_range(long i_small, long i_large);
