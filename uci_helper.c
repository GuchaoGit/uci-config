#include <stdio.h>
#include <stdlib.h>
#include <uci.h>
#include <string.h>
#include "uci_helper.h"

#define UCI_OK 0

int uci_set_option(char *pkt, char *sect, char *opt, char *value)
{
	if (pkt == NULL || sect == NULL || opt == NULL)
		return -1;
	printf("uci_set_option \n\t%s.%s.%s=%s\n", pkt, sect, opt, value);
	struct uci_context *_ctx = uci_alloc_context();
	struct uci_ptr ptr = {
		.package = pkt,
		.section = sect,
		.option = opt,
		.value = value,
	};
	uci_set(_ctx, &ptr);	//set config
	uci_commit(_ctx, &ptr.p, false);	//commit and save 
	uci_unload(_ctx, ptr.p);	//unload package
	uci_free_context(_ctx);	//release context
	return 0;
}

int uci_set_option_for_anonymous_section(char *ptr, char *value)
{
	if (ptr == NULL || value == NULL)
		return -1;
	struct uci_ptr p;
	struct uci_context *c;
	char *a = strdup(ptr);
	c = uci_alloc_context();	//apply UCI context
	if (uci_lookup_ptr(c, &p, a, true) != UCI_OK) {
		printf("uci get ptr failure!\n");
		uci_free_context(c);
		free(a);
		return -1;

	}

	printf("%s.%s.%s=%s\n", p.package,p.section, p.option, p.value);
	char *pkt =strdup(p.package); 
	char *sect =strdup(p.section);
       	char *opt = strdup(p.option);
	int rst = uci_set_option(pkt,sect,opt, value);
	uci_free_context(c); //release context
	free(a);
	free(pkt);
	free(sect);
	free(opt);
	return rst;

}

char *uci_get_option(char *ptr)
{

	if (ptr == NULL)
		return NULL;
	struct uci_ptr p;
	struct uci_context *c;
	char *a = strdup(ptr);

	c = uci_alloc_context();	//apply UCI context
	if (uci_lookup_ptr(c, &p, a, true) != UCI_OK) {
		printf("uci get option failure!%s\n",ptr);
		uci_free_context(c);
		free(a);
		return NULL;

	}
	char *result = strdup(p.o->v.string);
	uci_free_context(c);
	free(a);
	return result;
}
