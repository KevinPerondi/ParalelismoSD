#include <stdio.h>
#include <string.h>
 
int levenshtein(const char *s, const char *t)
{
	int ls = strlen(s), lt = strlen(t);
	int d[ls + 1][lt + 1];
 
	for (int i = 0; i <= ls; i++)
		for (int j = 0; j <= lt; j++)
			d[i][j] = -1;
 
	int dist(int i, int j) {
		if (d[i][j] >= 0) return d[i][j];
 
		int x;
		if (i == ls)
			x = lt - j;
		else if (j == lt)
			x = ls - i;
		else if (s[i] == t[j])
			x = dist(i + 1, j + 1);
		else {
			x = dist(i + 1, j + 1);
 
			int y;
			if ((y = dist(i, j + 1)) < x) x = y;
			if ((y = dist(i + 1, j)) < x) x = y;
			x++;
		}
		return d[i][j] = x;
	}
	return dist(0, 0);
}
 
int main(void)
{
	const char *s1 = "AVG Anti-Virus Free Edition 2012 review - PC Advisor: AVG Anti-Virus Free Edition 2012 reviewPC AdvisorBy Simon ... http://t.co/CVb5w1z3";
	const char *s2 = "Religious Sites Carry More Malware Than Porn Sites, Security Firm Reports http://t.co/A1E6zLRz";
	printf("distance between `%s' and `%s': %d\n", s1, s2,
		levenshtein(s1, s2));
 
        return 0;
}
