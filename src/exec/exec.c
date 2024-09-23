/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:31:57 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 09:21:54 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>


static void	handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node;
    
    node = cmd_node->content;
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->in);
    }
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->out);
    }
    else if (cmd_node->next)
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
    }
    close(pipes[PIPE_WRITE]);
}

void	handle_process(t_shell *shell, t_list *cmd, int fd[2])
{
    t_exec *node;
    int cmd_len;
    
    cmd_len = 0;
    node = cmd->content;
	if (node->args)
		cmd_len = ft_strlen(*node->args);
	handle_redirect(cmd, fd);
	close(fd[PIPE_READ]);
	builtin(shell,cmd,&g_env.exit_status,cmd_len);
	ft_lstclear(&shell->cmds, ft_free);
	exit(g_env.exit_status);
}

int check_fork(t_shell *shell, t_list *cmd, int fd[2])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		m_error(ERR_FORK, NULL, 1);
        return (0);
	}
	else if (pid == 0)
    {
		handle_process(shell, cmd, fd);
        exit(g_env.exit_status);
    }
    else
    return (1);
}
/*
void *prepare_exec(t_shell *shell, t_list *cmd, int fd[2])
{
	t_exec *cmd_info;
	DIR *dir;

    cmd_info = cmd->content;
    dir = NULL;
	if (cmd_info->args)
		dir = opendir(*cmd_info->args);
	if (cmd_info->in == -1 || cmd_info->out == -1)
	{
		if (dir)
			closedir(dir);
		return (NULL);
	}
	if ((cmd_info->args && access(cmd_info->path, X_OK) == 0) || built_check(cmd_info))
		check_fork(shell, cmd, fd);
	else if (cmd_info->path && (!access(cmd_info->path, F_OK) || dir))
		g_env.exit_status = 126;
	else if (cmd_info->args)
		g_env.exit_status = 127;
	if (dir)
		closedir(dir);
	return (NULL);
}


*/


