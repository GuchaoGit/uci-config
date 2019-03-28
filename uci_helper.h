#ifndef UCI_HELPER_H
#define UCI_HELPER_H

extern int uci_set_option(char *pkt, char *sect, char *opt,char *value);
extern int uci_set_option_for_anonymous_section(char *ptr, char *value);
extern char *uci_get_option(char *ptr);
	
#endif

