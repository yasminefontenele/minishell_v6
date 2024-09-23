/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/23 16:38:21 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"


void		type_of_separator(int *type_of, char **token)
{
    int i;

    i = 0;
    while (token[i])
    {
        if (token[i][0] != ' ' && token[i][0] != '\t')
            type_of[i] = -1;
        else
            type_of[i] = 1;
    i++;
    }
}

/*void filler_stokens(char **cmds, t_tokens **token, int nbr)
{
    int i;

    i = 0;
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    (*token)[i].type = set_token((*token)[i]);
    (*token)[i] = dollar_config((*token) +  i);

    (*token)[i] = dollar_config((*token) + i);

    
}*/

void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i, j;

    i = 0;
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    (*token)[i].type = set_token((*token)[i]);


    j = 0;
    while ((*token)[i].tokens[j])
    {
        if ((*token)[i].tokens[j][0] == '$')
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
        j++;
    }

    (*token)[i].tokens[j] = NULL;
}



void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    int         i;
    int         backslash;
    int         k;
    int         l;
    int         arg_count;

    t_tokens    *token;
    t_exec      *exec_node;
    t_list      *new_node;

    i = 1;
    k = 0;
    l = 0;
    arg_count = 0;
    token = malloc(sizeof(t_tokens) * i);
    if (!token)
        ft_error("Malloc failed in tokenize_commands\n", 1); 
    filler_stokens(cmdstr, &token, i, shell);

    backslash = valid_backslash(cmdstr);
    if (backslash == 0)
        ft_error("Invalid backslash\n", 1);
    if (syntax_grammar(cmdstr, token) == 1)
        exec_process_quotes(token, shell);
    exec_node = malloc(sizeof(t_exec));
    if (!exec_node)
        ft_error("Malloc failed for exec_node", 1);
    while (token[k].tokens[l])
    {
        arg_count += count_token(token[k].tokens[l]);
        l++;
    }
    exec_node->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!exec_node->args)
        ft_error("Malloc failed for exec_node->args", 1); 
    l = 0;
    while(token[k].tokens[l])
    {
        exec_node->args[l] = ft_strdup(token[k].tokens[l]);
        l++;
    }
    exec_node->args[l] = NULL;
    exec_node->path = NULL;
    exec_node->in = -1;
    exec_node->out = -1;
    new_node = ft_lstnew(exec_node);
    if (!new_node)
        ft_error("Malloc failed for new_node", 1);
    ft_lstadd_back(commands_list, new_node);
    free_tokens(token[k]);
    free(token[k].type);
    free(token);
}


