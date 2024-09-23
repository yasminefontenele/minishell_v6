/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:16:06 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 09:01:08 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char	**set_env(char *str, char *val, char **keys, int nbr)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*full_entry;
	
	if (nbr < 0)
		nbr = ft_strlen(str);	
	full_entry = ft_strjoin(str, "=");
	new_entry = ft_strjoin(full_entry, val);
	free(full_entry);
	i = 0;
	while (keys && keys[i])
	{
		key_len = second_strchr(keys[i], '=');
		if (ft_strncmp(keys[i], str, nbr) == 0 && (key_len >= nbr))
		{
			free(keys[i]);
			keys[i] = new_entry;
			return (keys);
		}
		i++;
	}
	keys = extend_form(keys, new_entry);
	return (keys);
}
// this function is used to get the value of an environment variable
char	*get_env(char *name, char **env, int name_len)
{
	int	i;
	int	n;

	i = 0;
	if (name_len < 0)
		name_len = ft_strlen(name);
	while (!ft_strchr(name, '=') && env && env[i])
	{
		n = name_len;
		if (n < second_strchr(env[i], '='))
			n = second_strchr(env[i], '=');
		if (!ft_strncmp(env[i], name, n))
			return (ft_substr(env[i], n + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}
/*
char *get_env(char *name, char **env, int name_len)
{
    int i;
    int n;

    i = 0;
    if (name_len < 0)
        name_len = ft_strlen(name);

    while (env && env[i])
    {
        n = name_len;
        if (n < second_strchr(env[i], '='))
            n = second_strchr(env[i], '=');
        if (!strncmp(env[i], name, n))
            return (ft_substr(env[i], n + 1, ft_strlen(env[i])));
        i++;
    }
    return (NULL);
}


char	*get_env(char *name, char **env, int name_len)
{
	int	i;
	int	equal_pos;

	i = 0;
	if (name_len < 0)
		equal_pos = ft_strlen(name);
	while (env && env[i])
	{
		equal_pos = second_strchr(env[i], '=');
		if (equal_pos > 0 && !ft_strncmp(env[i], name, name_len))
			return (ft_substr(env[i], equal_pos + 1, ft_strlen(env[i]) - equal_pos - 1));
		i++;
	}
	return (NULL);
}*/