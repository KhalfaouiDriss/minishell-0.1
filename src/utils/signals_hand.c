#include "../../include/minishell.h"

// int g_blocked = 0;

int global_state(int set)
{
	static int state = 0;

	if (set != -1)
		state = set;
	return state;
}

void get_sig(int sig)
{
	int state = global_state(-1);

	if (state == 1)
		return;
		
	if (sig == SIGINT)
		global_state(2);
	if(global_state(-1) != 3)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}