/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:20:02 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 18:23:40 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
/*
otimo, aparentemente está tudo ok!
só um comando foi alterado após essas trocas e gostaria que vc me axiliasse com isso.
no comando echo quando eu coloco juntamente com uma variavel ele não esta expandindo, por exemplo:
*/
int handle_basic_builtins(t_shell *shell, char **args)
{
    if (!ft_strncmp(args[0], "pwd", 3))
        return (m_pwd());
    else if (!ft_strncmp(args[0], "echo", 4))
		return (m_echo(args, shell));
	else if (!ft_strncmp(args[0], "unset", 5))
		return (m_unset(shell));
    return (-1); 
}

int builtin(t_shell *shell, t_list *cmd_ls, int *exit, int len)
{
    char **args;
    int builtin_result;

    while (cmd_ls)
    {
        args = ((t_exec *)cmd_ls->content)->args;
        len = args ? ft_strlen(*args) : 0;
        if (args && !ft_strncmp(args[0], "exit", 4) && len == 4)
            g_env.exit_status = m_exit(shell, cmd_ls, exit);
        else if (!cmd_ls->next && args && !ft_strncmp(args[0], "cd", 2) && len == 2)
            g_env.exit_status = m_cd(shell);
        else if (!cmd_ls->next && args && !ft_strncmp(args[0], "export", 6))
            g_env.exit_status = m_export(shell);
        else if (!cmd_ls->next && args && (builtin_result = handle_basic_builtins(shell,args)) != -1)
        {
            g_env.exit_status = builtin_result;
            return g_env.exit_status;
        }
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        cmd_execute(shell, cmd_ls);

        cmd_ls = cmd_ls->next;
    }
    return g_env.exit_status;
}

int built_check(t_exec *cmd)
{
	int	 i;

	if (!cmd->args)
		return (0);
	if ((cmd->args && ft_strchr(*cmd->args, '/')) || (cmd->args && ft_strchr((const char *)cmd->args, '/')))
		return (0);
	i = ft_strlen(*cmd->args);
	if (!ft_strncmp(*cmd->args, "pwd", i) && i == 3)
		return (1);
	if (!ft_strncmp(*cmd->args, "env", i) && i == 3)
		return (1);
	if (!ft_strncmp(*cmd->args, "cd", i) && i == 2)
		return (1);
	if (!ft_strncmp(*cmd->args, "export", i) && i == 6)
		return (1);
	if (!ft_strncmp(*cmd->args, "unset", i) && i == 5)
		return (1);
	if (!ft_strncmp(*cmd->args, "echo", i) && i == 4)
		return (1);
	if (!ft_strncmp(*cmd->args, "exit", i) && i == 4)
		return (1);
	return (0);
}
