/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:57:57 by emencova          #+#    #+#             */
/*   Updated: 2024/09/21 09:54:54 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define PIPE_READ	0
# define PIPE_WRITE	1
# define ERR_PIPEND	2
# define ERR_QUOTE	3
# define ERR_NEWDIR	4
# define ERR_NWPERM 5
# define ERR_NEWCMD	6
# define ERR_DUP	7
# define ERR_FORK	8
# define ERR_PIPE	9
# define ERR_MEM	10
# define ERR_ISDIR	11
# define ERR_NOTDIR	12

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/ioctl.h>
# include "../../libft/libft.h"
# include "../../minishell.h"


typedef struct s_shell
{
	t_list	*cmds; // linked list of commands
	char	**keys; // enviroment and keys
	pid_t	mpid; // main process id
}		t_shell;

typedef struct s_exec
{
	char	**args; // args for command
	char	*path;
	int		ac;
	int		in; // input file descriptor
	int		out; // output file descriptor
}		t_exec;

int		put_form_fd(char **form, int newline, int fd);
int		builtin(t_shell *shell, t_list *cmd_ls, int *e_xit, int len);
int		built_check(t_exec *cmd);
int		m_cd(t_shell *shell);
int		m_pwd();
int		m_echo(char **args);//, int ac);
int		error_unset(char *av);
int		m_exit(t_shell *shell, t_list *cmd, int *leave);
int 	find_key_idx(char **keys, char *key);
int 	m_unset(t_shell *shell);
int		m_export(t_shell *shell);
int 	create_here_document_fd(char *input_buffer[2], char *delimiter[2]);
int 	check_file_permissions(char *path, int is_write);
int 	open_file(char *path, int is_write, int is_append) ;
int 	ft_charstr(const char *str, char *set);
int		second_strchr(char *str, int c);
int		second_atoi(const char *str, long *n);
int		ft_isspace(const char c);
int		form_len(char **str);
int handle_basic_builtins(t_shell *shell, char **args);
void	free_form(char ***str);
void	m_perror(const char *msg, const char *detail, int exit_code);
void 	run_capture_output(char ***output, char *command_path, char *command_args, char **envp);
void	command_get(t_shell *shell, t_list *comnd);
void	cmd_execute(t_shell *shell, t_list *cmd);
void 	m_error(int error_type, char *limit, int status);
void 	error_cd(char **s[2]);
void	ft_free(void *content);
void	handle_process(t_shell *shell, t_list *cmd, int fd[2]);
int 	check_fork(t_shell *shell, t_list *cmd, int fd[2]);
void 	*prepare_exec(t_shell *shell, t_list *cmd, int fd[2]);
void 	close_old_fd(int oldfd);
t_exec 	*get_outfile(t_exec *node, char **args, int *i, int append);
t_exec 	*get_infile(t_exec *node, char **args, int *i, int here_doc);
char	**dupl_form(char **f);
char	**replace_form(char **big, char **small, int nbr);
char	**extend_form(char **insd, char *nstr);
char	*get_next_line(int fd);
char	**set_env(char *str, char *val, char **keys, int nbr);
char	*get_env(char *name, char **env, int name_len);
char 	*read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message);

void	m_getpid(t_shell *piid);
void	sigint_handler(int sig);
void	ft_exec(char ***out, char *full, char *ags, char **env);
#endif
