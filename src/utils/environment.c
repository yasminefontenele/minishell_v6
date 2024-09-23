/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:50:14 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/23 12:53:34 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//This function is used to duplicate the environment
char **dup_array(char **env)
{
    int i;
    char **sorted;

    i = 0;
    while (env[i])
        i++;
    sorted = malloc(sizeof(char *) * (i + 1));
    sorted[i] = NULL;
    i = -1;
    while (env[++i])
        sorted[i] = ft_strdup(env[i]);
    return sorted;
}

//This function is used to sort the environment
void sort_array(char **sorted)
{
    int    i;
    int    r;
    char   *tmp;

    tmp = NULL;
    r = 1;
    while(r)
    {
        i = -1;
        r = 0;
        while (sorted[++i])
        {
            if (sorted[i] && sorted[i + 1] &&
                ft_strncmp(sorted[i],sorted[i + 1],
                max_of(ft_strlen(sorted[i]),
                ft_strlen(sorted[i + 1]))) > 0)
            {
                tmp = sorted[i];
                sorted[i] = sorted[i + 1];
                sorted[i + 1] = tmp;
                r = 1;
            }
        }
    }
}

//This function is used to initialize the environment
void env_init(char **env)
{
    int i;
    char **sorted_env;

    i = 0;
    g_env.exit_status = 0;
    while(env[i])
        i++;
    g_env.env = malloc(sizeof(char *) * (i + 2));
    if (g_env.env == NULL)
        ft_error("malloc failed", 1);
    i = -1;
    g_env.env[++i] = ft_strdup("?=0");
    while (env[++i])
    {
        g_env.env[i] = ft_strdup(env[i - 1]);
        if (g_env.env[i] == NULL)
            ft_error("malloc failed", 1);
    }
    g_env.env[i] = NULL;
    sorted_env = dup_array(g_env.env);
    sort_array(sorted_env);
}

//adds a new environment variable to the g_env.env array
void append_to_env(char *variable, char *value, int size)
{
    int i;
    char	**updated_env;
	char	*new_env_var;
	char	*temp_str;

    i = 0;
    updated_env = malloc(sizeof(char *) * (size + 2));
    if (!updated_env)
        ft_error("malloc failed in append_to_env", 1);
    while (g_env.env[i])
    {
        updated_env[i] = ft_strdup(g_env.env[i]);
        i++;
    }
    temp_str = ft_strjoin(variable, "=");
    new_env_var = ft_strjoin(temp_str, value);
    free(temp_str);
    temp_str = NULL;
    updated_env[i++] = new_env_var;
    updated_env[i] = NULL;
    free_str_array(g_env.env);
    g_env.env = updated_env;
}