#include "execute.h"

void sigint_handler(int sig)
{
    (void)sig;

    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	m_getpid(t_shell *piid)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		m_error(ERR_FORK, NULL, 1);
		free_form(&piid->keys);
		exit(1);
	}
	if (!pid)
	{
		free_form(&piid->keys);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	piid->mpid = pid - 1;
}
