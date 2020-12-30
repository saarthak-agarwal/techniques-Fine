#include<stdio.h>
#include <stdlib.h>
int max(int a, int b)
{
	return a>b?a:b;
}
typedef struct pos
{
	int spos;
	int epos;
	int found;
} pos;
int match(char *pat, char *text, pos* ans, int tpos);
int match_star_greedy(int* characters, char* pat, char* text, pos* ans, int tpos)
{
	int f = 0;
	do
	{
		if(match(pat, text, ans, tpos+1))
		{
			ans->found = 1;
			f = 1;
		}
		tpos++;
	} while(*text != '\0' && (characters[*text++] || characters['.']));
	return f;
}

int match_star_nongreedy(int* characters, char* pat, char* text, pos* ans, int tpos)
{
	do
	{
		if(match(pat, text, ans, tpos+1))
		{
			ans->found = 1;
			return 1;
		}
		tpos++;
	} while(*text != '\0' && (characters[*text++] || characters['.']));
	return 0;
}

int match(char *pat, char *text, pos* ans, int tpos)
{
	// Base case
	if(*pat == '\0')
	{
		ans->epos = max(ans->epos, tpos-1);
		return 1;
	}

	int characters[257] = {0};
	characters[*pat] = 1;
	
	// character class
	if(*pat == '[')
	{
		int hyphen = 0;
		while(pat[0]!='\0' && pat[0]!=']')
		{
			characters[pat[0]]++;
			if(pat[0] == '-')
				hyphen++;
			else if(pat[0]=='z' && characters['a']>0 && hyphen>0)
			{
				for(int j=97; j<123; j++)
					characters[j]++;
				hyphen--;
			}
			else if(pat[0]=='Z' && characters['A']>0 && hyphen>0)
			{
				for(int j=65; j<91; j++)
					characters[j]++;
				hyphen--;
			}
			else if(pat[0]=='9' && characters['0']>0 && hyphen>0)
			{
				for(int j='0'; j<='9'; j++)
					characters[j]++;
				hyphen--;
			}
			pat = pat + 1;
		}
	}

	if(*pat == '\\' && pat[1] == 'w')
	{
		for(int j=97; j<123; j++)
			characters[j]++;
		for(int j=65; j<91; j++)
			characters[j]++;
		for(int j=48; j<=57; j++)
			characters[j]++;
		characters[95]++;
		pat = pat + 1;
	}

	if(*pat == '\\' && pat[1] == 'd')
	{
		for(int j=48; j<=57; j++)
			characters[j]++;
		pat = pat + 1;
	}

	// Zero or more - *
	if(*pat!='\0' && pat[1] == '*')
	{
		if(pat[2] == '?')
			return match_star_nongreedy(characters, pat+3, text, ans, tpos-1);
		else
			return match_star_greedy(characters, pat+2, text, ans, tpos-1);
	}

	// Zero or one - ?
	if(*pat!='\0' && pat[1] == '?')
	{
		pos inc = *ans, exc = *ans;
		int is_inc = 0, is_exc = 0; 
		if(characters[*text] || *pat == '.')
			is_inc = match(pat+2, text+1, &inc, tpos + 1);
		is_exc = match(pat+2, text, &exc, tpos);
		if(is_inc && is_exc)
		{
			if(inc.epos > exc.epos)
			{
				*ans = inc;
			}
			else
				*ans = exc;
			return 1;
		}
		else if(is_exc)
		{
			*ans = exc;
			return 1;
		}
		else if(is_inc)
		{
			*ans = inc;
			return 1;
		}
		else
			return 0;
	}

	// One or more - +
	if(*pat!='\0' && pat[1] == '+')
	{
		if(characters[*text] || *pat == '.')
			return match_star_greedy(characters, pat+2, text + 1, ans, tpos);
		else
			return 0;
	}

	// Anchor $
	if(pat[0] == '$' && pat[1] == '\0')
	{
		ans->epos = max(ans->epos, tpos);
		return *text == '\0';
	}

	// ONLY a single character match
	if(*text != '\0' && (*pat == '.' || characters[*text]))
	{
		ans->epos = max(ans->epos, tpos);
		tpos++;
		return match(++pat, ++text , ans, tpos);
	}
	return 0;	
}

pos match(char *pat, char *text)
{
	pos ans; 
	ans.found = 0; ans.spos = 0, ans.epos = -1;
	int tpos = 0;
	if(*pat == '^')
	{
		if(match(pat + 1, text, &ans, tpos))
		{
			ans.found = 1;
		}
		return ans;
	}
	do
	{
		if(match(pat, text, &ans, tpos))
		{
			ans.found = 1;
			return ans;
		}
		tpos++;
		ans.spos++;
	} 	while(*text++ != '\0');
	return ans;
}

int main()
{
	fflush(stdin);
	char* text = (char*)calloc(4100, sizeof(char));
	scanf("%[^\n]s", text);
	
	fflush(stdin);
	int number_of_tests; 
	scanf("%d", &number_of_tests);

	while(number_of_tests--)
	{
		fflush(stdin);
		char* pattern = (char*)calloc(4000, sizeof(char));
		scanf("%[^\n]s", pattern);
		
		pos ans = match(pattern, text);
		if(ans.found)
			printf("1 %d %d\n", ans.spos, max(ans.epos,0));
		else
			printf("0\n");
	}
}