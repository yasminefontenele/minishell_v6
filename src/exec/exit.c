/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:25 by emencova          #+#    #+#             */
/*   Updated: 2024/09/20 22:39:49 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int m_exit(t_shell *shell, t_list *cmd_node, int *leave)
{
    t_exec *cmd = (t_exec *)cmd_node->content;
    char *arg;
    int exit_value;
    int i;

    *leave = !shell->cmds->next;
    i = 0;
    if (!cmd->args || !cmd->args[1])
        exit(0);
    if (cmd->args[2])
    {
        *leave = 0;
        return (1);
    }
    arg = cmd->args[1];
    while (arg[i] != '\0')
    {
        if (!isdigit(arg[i]) && !(i == 0 && (arg[i] == '-' || arg[i] == '+')))
            exit(255);
        i++;
    }
    exit_value = atoi(arg);
    exit_value = exit_value % 256;
    if (exit_value < 0)
        exit_value += 256;
    exit(exit_value);
}