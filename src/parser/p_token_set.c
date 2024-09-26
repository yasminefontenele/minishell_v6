/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/26 21:15:49 by yfontene         ###   ########.fr       */
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

void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell) {
    int i = 0, j = 0;

    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);

    printf("command tokens %s:\n", *cmds);
    while ((*token)[i].tokens[j])
    {
        printf("Token %d: %s\n", j, (*token)[i].tokens[j]);
        j++;
    }
    if (!(*token)[i].tokens) {
        ft_error("Token split failed\n", 1);
        return;
    }

    (*token)[i].type = set_token((*token)[i]);

    j = 0;
    while ((*token)[i].tokens[j]) {
        if ((*token)[i].tokens[j][0] == '$')
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
        j++;
    }

    (*token)[i].tokens[j] = NULL;
}

char **split_pipes(char *line)
{
    char **cmds;
    
    cmds = ft_split(line, '|');
    if (!cmds)
        ft_error("Failed to split commands by pipe\n", 1);
    return cmds;
}

void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    int i = 0;
    char **cmds;
    t_tokens *token;
    t_exec *exec_node;
    t_list *new_node;

    cmds = split_pipes(*cmdstr);
    
    printf("Commands divided by pipes:\n");
    for (i = 0; cmds[i] != NULL; i++) {
        printf("Commands %d: %s\n", i, cmds[i]);
    }
    i = 0;

    while (cmds[i]) {
        token = malloc(sizeof(t_tokens));
        if (!token)
            ft_error("Malloc failed in tokenize_commands\n", 1); 
            
        filler_stokens(&cmds[i], &token, i, shell);

        exec_node = malloc(sizeof(t_exec));
        if (!exec_node)
            ft_error("Malloc failed for exec_node", 1);
            
        exec_node->args = malloc(sizeof(char *) * (count_token(cmds[i]) + 1));
        if (!exec_node->args)
            ft_error("Malloc failed for exec_node->args", 1);
            
        int j = 0;
        while (token->tokens[j]) {
            exec_node->args[j] = ft_strdup(token->tokens[j]);
            j++;
        }
        exec_node->args[j] = NULL;
        new_node = ft_lstnew(exec_node);
        if (!new_node)
            ft_error("Malloc failed for new_node", 1);
        ft_lstadd_back(commands_list, new_node);
        free_tokens(token);
        free(token->type);
        free(token);
        i++;
    }
    free_split(cmds);
}


