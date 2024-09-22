/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:09 by emencova          #+#    #+#             */
/*   Updated: 2024/09/21 14:31:37 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
/*
int	m_cd(t_shell *shell)
{
	char	**str[2];
	char	*home_dir;
	char	*current_dir;

	g_env.exit_status = 0;
	str[0] = ((t_exec *)shell->cmds->content)->args;
	home_dir = get_env("HOME", shell->keys, 4);
	if (!home_dir)
		home_dir = ft_strdup(" ");
	str[1] = extend_form(NULL, home_dir);
	free(home_dir);
	current_dir = getcwd(NULL, 0);
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	error_cd(str);
	if (!g_env.exit_status)
		shell->keys = set_env("OLDPWD", str[1][1], shell->keys, 6);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		current_dir = ft_strdup(" ");
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	shell->keys = set_env("PWD", str[1][2], shell->keys, 3);
	free_form(&str[1]);
	return (g_env.exit_status);
}*/
//ULTIMA VESAO FUNCIONAL
/*int m_cd(t_shell *shell) {
    char **str;
    char *target_dir;
    char *current_dir;

    g_env.exit_status = 0;
    str = ((t_exec *)shell->cmds->content)->args;

    if (!str[1] || ft_strcmp(str[1], "") == 0)
	{
        target_dir = get_env("HOME", shell->keys, 4);
        if (!target_dir)
            target_dir = "/";
    } 
	else 
        target_dir = str[1];
    if (chdir(target_dir) == -1)
	{
        perror("cd");
        g_env.exit_status = 1;
    }
	else
	{
        current_dir = getcwd(NULL, 0);
        if (current_dir)
		{
            shell->keys = set_env("PWD", current_dir, shell->keys, 3);
            free(current_dir);
        }
    }

    return (g_env.exit_status);
}*/

int m_cd(t_shell *shell) {
    char **str;
    char *target_dir;
    char *current_dir;
    char cwd_buffer[PATH_MAX];


    g_env.exit_status = 0;
    str = ((t_exec *)shell->cmds->content)->args;
    if (!str[1] || ft_strcmp(str[1], "") == 0)
    {
		

        target_dir = get_env("HOME", shell->keys, 4);
        if (!target_dir || ft_strcmp(target_dir, "") == 0)
        {
            fprintf(stderr, "Erro: Variável de ambiente HOME não está definida\n");
            target_dir = "/";
        }
		printf("Tentando mudar para o diretório: %s\n", target_dir);
    } 
    else
	{
		target_dir = str[1];
	} 
    if (chdir(target_dir) == -1)
    {
        perror("cd");
        g_env.exit_status = 1;
    }
    else
    {
        current_dir = getcwd(cwd_buffer, sizeof(cwd_buffer));
        if (current_dir)
        {
            shell->keys = set_env("PWD", current_dir, shell->keys, 3);
        }
        else
        {
            perror("getcwd");
            g_env.exit_status = 1;
        }
    }

    return (g_env.exit_status);
}


int	m_pwd(void)
{
	char *buffer;

    buffer = getcwd(NULL, 1024);  // Altere 0 para um valor padrão, como 1024
    if (!buffer)
    {
        perror("pwd");
        return (1);
    }
    ft_putendl_fd(buffer, 1);
    free(buffer);
    return (0);
}
/*
int	m_echo(char **args, int ac)
{
	int i;
	int newline;

	
	i = 1;
	newline = 1;
	if (!args || !args[0])
		return (1);
	if (ac > 1 && args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (i < ac && args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (i < ac - 1)
			write(STDOUT_FILENO, " ", 1); 
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}



int	m_echo(char **args)
{
	char	**path;
	int		i;

	path = args;
	i = 1;
	if (*args && args[0][0] == '-')
		while (args[0][i] == 'n')
			i++;
	if (*args && (ft_strncmp(*args, "-n", 2) == 0) && !args[0][i])
		path++;
	while (*path)
	{
		ft_putstr_fd(*path, STDOUT_FILENO);
		path++;
		if (*path)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!(*args && ft_strncmp(*args, "-n", 2) == 0 && !args[0][i]))
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
*/

/*int m_echo(char **args)
{
    char **path;
    int i;

    path = args;
    i = 1;
    if (*args && args[0][0] == '-')
    {
        while (args[0][i] == 'n')
            i++;
    }
    if (*args && (ft_strncmp(*args, "-n", 2) == 0) && !args[0][i])
        path++;
    if (*path)
        path++;
    while (*path)
    {
        ft_putstr_fd(*path, STDOUT_FILENO);
        path++;
        if (*path)
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!(*args && ft_strncmp(*args, "-n", 2) == 0 && !args[0][i]))
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}*/

//function changed to not print -n and not skip line
int m_echo(char **args)
{
    int i;
    int no_newline;

    i = 1;
	no_newline = 0;
    if (args[i] && strcmp(args[i], "-n") == 0)
	{
        no_newline = 1;
        i++;
    }
    while (args[i])
	{
        ft_putstr_fd(args[i], STDOUT_FILENO);
        i++;
        if (args[i])
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!no_newline) {
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
    return 0;
}

