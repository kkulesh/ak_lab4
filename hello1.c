#include <hello1.h>

MODULE_AUTHOR("IO-14 Kateryna Kulesh");
MODULE_DESCRIPTION("lab4");
MODULE_LICENSE("Dual BSD/GPL");

struct timeit_list {
	struct list_head node;
	ktime_t before;
	ktime_t after;
};

static struct list_head head_node = LIST_HEAD_INIT(head_node);


int print_hello(uint amount)
{
	struct timeit_list *list, *tmp;
	uint i;

	if (amount < 0) {
		pr_err("Fail! The parameter's less than 0\n");
	} else if (amount == 0) {
		pr_warn("Warning! The parameter's 0\n\n");
	} else if (amount >= 5 && amount <= 10) {
		pr_warn("Warning! The parameter's between 5 and 10\n");
	} else if (amount > 10) {
		pr_err("Fail! The parameter's > 10\n");
		return -EINVAL;
	}

	for (i = 0; i < amount; i++) {
		list = kmalloc(sizeof(struct timeit_list), GFP_KERNEL);
		if (ZERO_OR_NULL_PTR(list))
			goto tidy_away;
		list->before = ktime_get();
		pr_info("Hello, world!\n");
		list->after = ktime_get();
		list_add_tail(&list->node, &head_node);
	}
	return 0;

tidy_away:
	list_for_each_entry_safe(list, tmp, &head_node, node) {
		list_del(&list->node);
		kfree(list);
	}
	pr_err("Fail! No memory\n");
	return -1;
}
EXPORT_SYMBOL(print_hello);


static int __init init_hello(void)
{
	pr_info("Success! hello1 has been initiated\n");
	return 0;
}


static void __exit exit_hello(void)
{
	struct timeit_list *list, *tmp;

	list_for_each_entry_safe(list, tmp, &head_node, node) {
		pr_info("Time: %lld", list->after - list->before);
		list_del(&list->node);
		kfree(list);
	}
	BUG_ON(!list_empty(&head_node));
	pr_info("Success! hello1 has been unloaded\n");
}


module_init(init_hello);
module_exit(exit_hello);
