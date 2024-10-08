/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:34:30 by emencova          #+#    #+#             */
/*   Updated: 2024/09/21 06:35:54 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../../minishell.h"
#include <dirent.h>


static char	*cmd_find(char **path_env, char *comnd, char *path_full)
{
	char	*tmp;
	int		i;

	i = -1;
	path_full = NULL;
	while (path_env && path_env[++i])
	{
		free(path_full);
		tmp = ft_strjoin(path_env[i], "/");
		if (!tmp)
			return (NULL);
		path_full = ft_strjoin(tmp, comnd);
		free(tmp);
		if (!path_full)
			return (NULL);
		if (access(path_full, F_OK) == 0)
			break ;
	}
	if (!path_env || !path_env[i])
	{
		free(path_full);
		return (NULL);
	}
	return (path_full);
}

static DIR	*check_cmd(t_shell *shell, t_list *comnd, char ***str)
{
	t_exec	*node;
	DIR		*directory;
	char	*path_env;

	directory = NULL;
	node = comnd->content;
	if (node && node->args)
		directory = opendir((const char *)node->args);
	if (node && node->args && ft_strchr(*node->args, '/') && !directory)
	{
		*str = ft_split(*node->args, '/');
		node->path = ft_strdup(*node->args);
		free(node->args[0]);
		node->args[0] = ft_strdup(str[0][form_len(*str) - 1]);
	}
	else if (!built_check(node) && node && node->args && !directory)
	{
		path_env = get_env("PATH", shell->keys, 4);	
		*str = ft_split(path_env, ':');
		if (!*str)
   			 m_error(ERR_NEWCMD, "Failed to split PATH", 127);
		free(path_env);
		node->path = cmd_find(*str, *node->args, node->path);
		if (!node->path || !node->args[0] || !node->args[0][0])
				m_error(ERR_NEWCMD, *node->args, 127);
	}
	return (directory);
}


void command_get(t_shell *shell, t_list *comnd) {
    t_exec *node;
    DIR *directory;
    char **str;

    str = NULL;
    node = comnd->content;
    if (built_check(node)) {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory) {
        closedir(directory);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
	{
        pid_t pid = fork();
        if (pid < 0)
            m_error(ERR_FORK, NULL, 1);
        else if (pid == 0)
		{
            execve(node->path, node->args, shell->keys);
            m_error(ERR_ISDIR, node->args[0], 126);
        }
		else
            waitpid(pid, &g_env.exit_status, 0);
	}
    else 
        m_error(ERR_NEWDIR, node->args[0], 127);
    free_form(&str);
}

void cmd_execute(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    t_list  *cmd_node;
    t_exec  *current_cmd;
    t_exec  *next_cmd;

    cmd_node = commands_list;
    while (cmd_node)
    {
        current_cmd = (t_exec *)cmd_node->content;
        if (!cmd_node || !current_cmd)
        {
            fprintf(stderr, "Erro: cmd_node ou current_cmd está nulo em cmd_execute\n");
            return;
        }
        if (cmd_node->next)
            next_cmd = (t_exec *)cmd_node->next->content;
        else
            next_cmd = NULL;
        command_get(shell, cmd_node);
        if (next_cmd)
        {
            if (pipe(fd) == -1)
                m_error(ERR_PIPE, NULL, 1);
            if (!check_fork(shell, cmd_node, fd))
                return;
            close(fd[PIPE_WRITE]);
            if (!next_cmd->out)
                next_cmd->in = fd[PIPE_READ];
            if (next_cmd && next_cmd->out)
                close(fd[PIPE_READ]);
        }
        else if (current_cmd->in == 2)
            close(current_cmd->in);
        if (current_cmd->out > 2)
            close(current_cmd->out);
        cmd_node = cmd_node->next;
    }
}

