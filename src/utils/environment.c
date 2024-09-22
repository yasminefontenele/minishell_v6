/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:50:14 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/05 19:39:16 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//This function is used to duplicate the environment
void dup_array(void)
{
    int i;

    i = 0;
    while (g_env.env[i])
        i++;
    g_env.sorted = malloc(sizeof(char *) * (i + 1));
    g_env.sorted[i] = NULL;
    i = -1;
    while (g_env.env[++i])
        g_env.sorted[i] = ft_strdup(g_env.env[i]);
}

//This function is used to sort the environment
void sort_array(void)
{
    int    i;
    int    r;
    char   *tmp;

    tmp = NULL;
    r = 1;
    dup_array();
    while(r)
    {
        i = -1;
        r = 0;
        while (g_env.sorted[++i])//sort the environment variables
        {
            if (g_env.sorted[i] && g_env.sorted[i + 1] &&
                ft_strncmp(g_env.sorted[i],g_env.sorted[i + 1],
                max_of(ft_strlen(g_env.sorted[i]),
                ft_strlen(g_env.sorted[i + 1]))) > 0)
            {
                tmp = g_env.sorted[i];
                g_env.sorted[i] = g_env.sorted[i + 1];
                g_env.sorted[i + 1] = tmp;
                r = 1;
            }
        }
    }
}

//This function is used to initialize the environment
void env_init(char **env)
{
    int i;
    
    i = 0;
    g_env.exit_status = 0;//initialize the exit status
    while(env[i])//count the number of environment variables
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
    sort_array();
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