/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:49 by emencova          #+#    #+#             */
/*   Updated: 2024/09/19 23:35:48 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int find_key_idx(char **keys, char *key)
{
    int i ;
    int key_len; 

	i = 0;
	key_len= second_strchr(key, '=');
    if (key_len == -1)
        return -1;
    while (keys[i])
    {
        if (!ft_strncmp(keys[i], key, key_len + 1))
            return (i);
        i++;
    }

    return (-1);
}

int m_unset(t_shell *shell)
{
    char **av;
	char *var_env;
    int i;
    int index;

	i = 1;
	av = ((t_exec *)shell->cmds->content)->args;
    if (form_len(av) < 2)
        return 0;
    while (av[i])
    {
        if (av[i][ft_strlen(av[i]) - 1] != '=')
        {
            var_env = ft_strjoin(av[i], "=");
            free(av[i]);
            av[i] = var_env;
        }
        index = find_key_idx(shell->keys, av[i]);
        if (index != -1)
            replace_form(shell->keys, NULL, index);
        i++;
    }
    return 0;
}

int	m_export(t_shell *shell)
{
	int		i;
	int		index;;
	char	**av;

	av = ((t_exec *)shell->cmds->content)->args;
	i = 1;
	if (form_len(av) < 2)
		return (0);
	while (av[i])
	{
		index = find_key_idx(shell->keys, av[i]);
		if (index != -1)
		{
			free(shell->keys[index]);
			shell->keys[index] = ft_strdup(av[i]);
		}
		else
			shell->keys = extend_form(shell->keys, av[i]);
		i++;
	}
	return (0);
}
