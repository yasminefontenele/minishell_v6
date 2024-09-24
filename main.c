/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:01:48 by emencova          #+#    #+#             */
/*   Updated: 2024/09/24 12:51:45 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./src/exec/execute.h"

t_env   g_env;
/*
static t_shell	var_init(t_shell shell, char *s, char **av)
{
	char	*nbr;

	s = getcwd(NULL, 0);
	shell.keys = set_env("PWD", s, shell.keys, 3);
	free(s);
	s = get_env("SHLVL", shell.keys, 5);
	if (!s || ft_atoi(s) <= 0)
		nbr = ft_strdup("1");
	else
		nbr = ft_itoa(ft_atoi(s) + 1);
	free(s);
	shell.keys = set_env("SHLVL", nbr, shell.keys, 5);
	free(nbr);
	s = get_env("PATH", shell.keys, 4);
	if (!s)
		shell.keys = set_env("PATH", \
		"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", shell.keys, 4);
	free(s);
	s = get_env("_", shell.keys, 1);
	if (!s)
		shell.keys = set_env("_", av[0], shell.keys, 1);
	free(s);
	return (shell);
}

static t_shell	prompt_init(char **av, char **env)
{
	t_shell	shell;
	char		*s;

	s = NULL;
	shell.cmds = NULL;
	shell.keys = dupl_form(env);
	g_env.exit_status = 0;
	m_getpid(&shell);
	shell = var_init(shell, s, av);
	return (shell);
}
*/
int main(int ac, char **av, char **envp)
{
    t_shell shell;
    char    *line;
    t_list  *command_list;
	
    (void)ac;
	(void)av;
	
    command_list = NULL;
    line = NULL;
	env_init(envp, &shell);
   // shell = prompt_init(envp, av);
//	char *test_var = get_env("HOME", shell.keys, 4);
//	printf("HOME in main function after var init is : %s\n", test_var ? test_var : "not found");
//	free(test_var);
    while (1)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT,SIG_IGN);
		line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
        {
            add_history(line);
            tokenize_commands(&line, &command_list, &shell);
        }  
        shell.cmds = command_list;
        cmd_execute(&shell, shell.cmds);
		ft_lstclear(&command_list, free);
		command_list = NULL;
        free(line);
    }
    return (g_env.exit_status);
}
