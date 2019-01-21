#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <uci.h>
#define UCI_OK 0
#define UCI_CONFIG_FILE  "network"

static struct uci_context *ctx= NULL;//定义uci上下文的静态变量
const char *value;
char *apn;
void load_config()
{

	struct uci_package* pkg = NULL;
	struct uci_element *e,*oe;
	ctx = uci_alloc_context();//申请UCI上下文
	int result;
	if((result=uci_load(ctx,UCI_CONFIG_FILE,&pkg))!=UCI_OK){
		printf("result code = %d\n",result);
		printf("open config file failure\n");
		goto cleanup;//打开uci文件失败，则跳到文末 清理uci上下文
	}
	printf("open config file success\n");


	uci_foreach_element(&pkg->sections,e){//遍历所有的section
		struct uci_section *s = uci_to_section(e);//将element转换为section类型

		printf("uci type = %d ,uci name =  %s\n",(s->e).type,(s->e).name);


		//遍历section下options
		uci_foreach_element (&s->options,oe){
			struct uci_option *o = uci_to_option(oe);
			printf("uci type = %d ,uci name =  %s\n",(o->e).type,(o->e).name);
		
			if(NULL != (value = uci_lookup_option_string(ctx,s,(o->e).name))){
				printf("%s = %s\n\n",(o->e).name,value);
			}

		}
		printf("\n\n");

	}

	uci_unload(ctx,pkg);//release package

cleanup:
	uci_free_context(ctx);
	ctx = NULL;
}

int modifyApn(const char*section,char *apn){

	struct uci_context * _ctx = uci_alloc_context(); //申请上下文
	struct uci_ptr ptr ={
		.package = "network",
		.section = section,
		.option = "apn",
		.value = apn,
	};
	uci_set(_ctx,&ptr); //写入配置
	uci_commit(_ctx, &ptr.p, false); //提交保存更改
	uci_unload(_ctx,ptr.p); //卸载包
	uci_free_context(_ctx); //释放上下文

	return 0;//modify success
}

int main(void)
{
	load_config();

//	modifyApn("wan","myapn");

//	load_config();


	return 0;
}
