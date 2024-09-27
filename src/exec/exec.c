/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:31:57 by emencova          #+#    #+#             */
/*   Updated: 2024/09/27 22:21:24 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>

void reset_command_state(t_list *cmd_node)
{
    t_exec *node = (t_exec *)cmd_node->content;

    node->in = STDIN_FILENO;
    node->out = STDOUT_FILENO;
}

void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];               // Pipe file descriptors
    int in_fd = STDIN_FILENO; // Track input for the next command
    t_list *cmd_node = commands_list;

    while (cmd_node)
    {
        reset_command_state(cmd_node);
        printf("in exec pipe while loop \n");

        // If there's another command, create a pipe
        if (cmd_node->next)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) // Child process
        {
            // Handle input redirection from the previous command
            if (in_fd != STDIN_FILENO)
            {
                if (dup2(in_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 in_fd");
                    exit(EXIT_FAILURE);
                }
                close(in_fd); // Close the old file descriptor
            }

            // Handle output redirection to the pipe (if not the last command)
            if (cmd_node->next)
            {
                if (dup2(fd[PIPE_WRITE], STDOUT_FILENO) == -1)
                {
                    perror("dup2 pipe write");
                    exit(EXIT_FAILURE);
                }
                close(fd[PIPE_WRITE]);
                close(fd[PIPE_READ]);
            }

            // Execute the command
            command_get_pipeline(shell, cmd_node);
            exit(EXIT_SUCCESS);
        }
        else // Parent process
        {
            // Close the write end of the pipe, we're done writing
            if (cmd_node->next)
                close(fd[PIPE_WRITE]);

            // Close the previous input file descriptor if not STDIN
            if (in_fd != STDIN_FILENO)
                close(in_fd);

            // Move to the next command in the pipeline
            if (cmd_node->next)
                in_fd = fd[PIPE_READ]; // The next command reads from the pipe

            cmd_node = cmd_node->next;
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
}

void handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node = (t_exec *)cmd_node->content;

    printf("inside handle redirect \n");
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            perror("Error duplicating input");
            exit(EXIT_FAILURE);
        }
        close(node->in);
    }
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            perror("Error duplicating output");
            exit(EXIT_FAILURE);
        }
        close(node->out);
    }
    else if (cmd_node->next)
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            perror("Error duplicating pipe write end");
            exit(EXIT_FAILURE);
        }
        close(pipes[PIPE_WRITE]);
    }

    if (cmd_node->next)
        close(pipes[PIPE_READ]);

    printf("leaving handle_redirect\n");
}
