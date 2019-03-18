#include	"unp.h"

int main(int argc, char **argv)
{
	char			*ptr, **pptr;
	char			str[INET6_ADDRSTRLEN];
	struct hostent	*hptr, *hhptr;

	while (--argc > 0) 
	{
		ptr = *++argv;
		if ( (hptr = gethostbyname(ptr)) == NULL) 
		{
			err_msg("gethostbyname error for host: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);

		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("	alias: %s\n", *pptr);

		switch (hptr->h_addrtype) 
		{
		case AF_INET:
#ifdef	AF_INET6
		case AF_INET6:
#endif
			pptr = hptr->h_addr_list;
			for ( ; *pptr != NULL; pptr++) 
			{
				printf("\taddress: %s\n", Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));

				//如果反向解析失败，则可能是该电脑没有反向解析的功能
				if ( (hhptr = gethostbyaddr(*pptr, hptr->h_length, hptr->h_addrtype)) == NULL)
					printf("\t(gethostbyaddr failed)\n");
				else if (hhptr->h_name != NULL)
					printf("\tname = %s\n", hhptr->h_name);
				else
					printf("\t(no hostname returned by gethostbyaddr)\n");
			}
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(0);
}
