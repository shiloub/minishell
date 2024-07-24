/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clmurphy <clmurphy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 13:33:36 by amontant          #+#    #+#             */
/*   Updated: 2022/06/08 10:06:47 by clmurphy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "../GNL/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <signal.h>
# include "libft.h"
# include "minishell.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define KEY_UP 38

struct			s_command;
struct			s_mshell;
extern int		g_estatus;

typedef struct s_redir_in
{
	char				*name;
	int					type;
	char				*stop;
	struct s_redir_in	*next;
}	t_redir_in;

typedef struct s_redir_out
{
	char				*name;
	int					append;
	struct s_redir_out	*next;
}	t_redir_out;

typedef struct s_command
{
	char				**value;
	t_redir_out			*out;
	t_redir_in			*in;
	struct s_command	*next;
}	t_command;

typedef enum e_token
{
	string = 0,
	builtin = 1,
	flag = 2,
	PIPE = 3,
	redir_in = 4,
	redir_out = 5,
	d_variable = 6,
	here_doc = 7,
	here_doc_del = 8,
	redir_app = 9,
}	t_token;

typedef struct s_split
{
	char			c;
	struct s_split	*next;
}	t_split;

typedef struct s_pipe
{
	int					pipe_fd[2];
	struct s_pipe		*next;
}	t_pipe;

typedef struct s_phrase
{
	char			*str;
	struct s_phrase	*next;
}	t_phrase;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_mshell
{
	int			*pipe_fd;
	t_env		*env;
	char		**path;
	int			*pids;
	char		*res;
	char		*var;
	int			j;
	int			hdoc_flag;
	int			s_error;
	t_command	*command;
	t_phrase	*phrase;
	t_split		*word;
}	t_mshell;

/**************MAIN*****************/
int				ft_read(char *str, t_mshell *mshell);
void			free_mshell(t_mshell *mshell, char *str);

/**************PARSE****************/

int				parse_command(char *str, t_mshell *mshell);
t_command		*create_command(t_mshell *mshell);
int				parse_command_list(t_phrase *temp_phrase, \
			t_command **command);
int				is_delim(t_mshell *mshell, char *str, int *i);
int				is_redir(char *str, int *i, t_mshell *mshell);
int				init_values(int	*i, int *size, t_command **temp_command, \
				t_phrase **phrase);
/**************PARSE_UTLIS****************/
int				check_args(int ac, char **av);
int				next_phrase(int *i, t_phrase **temp_phrase, \
t_command *temp_command);
int				next_command(int *i, t_phrase **temp_phrase, \
t_command **temp_command, t_command **command);
char			*ft_strjoin_f2(char *s1, char const *s2);

/**************PARSE_QUOTE****************/
int				empty_quote(int type, t_mshell *mshell, char *str, int *i);
int				quote_error(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				parse_if_word(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				dollar_in_quote_string(t_split **word, t_mshell *mshell, \
			char *str, int *i);
char			*delim_q(char	*str);

/*************ENV_LIST****************/
void			print_env(t_env *env);
void			del_one(t_env **lst, t_env *to_del);
t_env			*env_new(char *name, char *value);
void			env_add_back(t_env **alst, t_env *new_elem);
void			env_free(t_env *env);

/************ENV*************************/
t_env			*parse_env(char **env_t);
void			parse_quotes(t_split **word, t_mshell *mshell, \
				char *str, int *i);
void			check_serror(t_phrase **phrase, t_mshell *mshell);
/************PARSE_VARS BIS*******************/
int				define_quote_type(char	*str, int *i);
int				dollar_only(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				parse_dollar_string(int *j, t_mshell *mshell, \
				char *str, int *i);
int				dollar_only(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				quote_after_dollar(t_split **word, t_mshell *mshell, \
			char *str, int *i);
int				not_quotes(int type, char *str, int *i);
/************PARSE_VARS*******************/
int				parse_dquote_string_bis(int type, t_mshell *mshell, \
				char *str, int *i);
int				dquotes_only(int type, t_mshell *mshell, char *str, int *i);
int				eq_start_dquote(t_mshell *mshell, char *str, int *i);
int				digit_in_dquote(t_mshell *mshell, char *str, int *i);
int				parse_dquote_string(int type, t_mshell *mshell, \
				char *str, int *i);

/************PARSE_DQUOTES_BIS*******************/
int				eq_in_dquote(t_mshell *mshell, char *str, int *i);
int				check_dquote_error(char *str, int *i);
int				check_dquote_in_env(int type, t_mshell *mshell, \
				char *str, int *i);

/***********PARSE_STRING*******************/
void			split_command(char *str, t_mshell *mshell);
char			*make_word(t_split **word, t_mshell *mshell);
int				parse_string(t_split **word, t_mshell *mshell, \
				char *str, int *i);
void			parse_delimiter(t_mshell *mshell, \
				char *str, int *i);
void			parse_string_bis(t_split **word, t_mshell *mshell, \
				char *str, int *i);
void			new_word_afte_pipe(t_split **delimiter, t_mshell *mshell);
int				check_redirs(t_mshell *mshell, char *str, int *i);
int				is_redir2(char *str, int *i, t_mshell *mshell);
int				check_redirs2(t_mshell *mshell, char *str, int *i);
/***********PARSE_DELIM_UTILS*******************/
int				parse_redir(t_split **delimiter, t_mshell *mshell, \
			char *str, int *i);
int				is_pipe(t_split **delimiter, t_mshell *mshell, \
				char *str, int *i);
int				end_of_string(t_split **delimiter, t_mshell *mshell, \
				char *str, int *i);

/***********PARSE_DOLLAR*******************/
void			parse_dollar(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				number_after_dollar(t_mshell *mshell, char *str, int *i);
int				if_another_dollar(t_split **word, t_mshell *mshell, \
			char *str, int *i);
int				make_var(t_mshell *mshell, char *str, int *i);

/***********PARSE_DOLLAR_UILS*******************/
char			*is_in_env(t_mshell *mshell, char *str);
char			*ft_strndup(const char *s, int size);
void			parse_dollar_dquotes(int type, t_mshell *mshell, \
				char *str, int *i);
char			*ft_strndup2(const char *s, int size);
int				is_delim_dollar(char *str, int i);
void			make_var2(t_mshell *mshell, char *str, int *i);
/***********PARSE_DOLLAR_UILS2*******************/
int				parse_quotes_in_destring(int *type, t_mshell \
			*mshell, char *str, int *i);
int				quotes_in_dstring(int *type, t_mshell *mshell, \
				char *str, int *i);
int				eq_in_dollar(int *typej, t_mshell *mshell, char *str, int *i);
int				another_dollar(int *j, t_mshell *mshell, char *str, int *i);
int				eq_in_pdollar(t_mshell *mshell, char *str, int *i);
/************PARSE_DQUOTES_UTILS*******************/
int				dquote_is_digit(t_mshell *mshell, char *str, int *i);
int				parse_dquote_string2(int type, t_mshell *mshell, \
				char *str, int *i);
int				dquote_eq(t_mshell *mshell, char *str, int *i);
void			single_quote_var(t_mshell *mshell, char *str, int *i);

/***********SPLIT_UTILS*******************/
t_split			*split_lstnew(char c);
void			split_lstadd_back(t_split **alst, t_split *new);
int				split_lstsize(t_split *lst);
void			phrase_lstadd_back(t_phrase **alst, t_phrase *new);
void			print_split(t_split **split);
char			*hdoc_expand(char *str, t_mshell *mshell);
void			if_word_free_word(t_split **word);
/***********WORD_UTILS*******************/
void			print_phrase(t_phrase **phrase);
void			ft_wordclear(t_split **lst);
t_phrase		*phrase_lstnew(char *str);
int				phrase_lstsize(t_phrase *lst);
void			free_phrase(t_phrase **phrase);
void			free_phrase2(t_phrase **phrase);
/************COMMAND_UTILS**********/
void			command_lstadd_back(t_command **alst, t_command *new);
t_command		*command_lstnew(char **commands);
void			print_tab(char **tab);
int				count_delim(t_phrase **phrase);
void			freetab(char **tab);
void			print_command(t_command *command);
int				command_size(t_command *command);
void			free_command(t_command **command);
/**************INIT*****************/
t_mshell		*init_mshell(char **env);
void			syntax_error(t_mshell *mshell);
/**************ARM_SIGNAL************/
void			armsignals(void);
void			signal_def(void);
void			end_signals(void);
/************ASSIGN_TOKENS***********/
void			assign_tokens(t_mshell *mshell);
int				is_variable(char *str);
int				is_builtin(char *str);
int				parse_string2(t_split **word, t_mshell *mshell, \
				char *str, int *i);
void			parse_string_bis2(t_split **word, t_mshell *mshell, \
				char *str, int *i);
int				quote_delim(int type, t_mshell *mshell, char *str, int *i);
int				quote_delim2(int type, t_mshell *mshell, char *str, int *i);
int				quote_delim3(t_mshell *mshell, char *str, int *i);
void			if_word(t_split **word, t_mshell *mshell);
char			*hdoc_expand2(char *str, char *s1, t_mshell *mshell);

/************BUILTINS*****************/
void			del_one(t_env **lst, t_env *to_del);
char			*get_value(char *line);
char			*get_name(char *line);
int				export_variable(t_env **env, char *new_v);
void			ft_export(t_env **env, char **params);
int				simul_ft_export(t_env	**env, char **params);
int				check_valid_variable(char *variable);
void			check_rm_double(t_env **env);
void			ft_unset(t_env **env, char **params);
void			unset_variable(t_env **env, char *variable_name);
void			cd(char **params);
void			pwd(void);
int				is_valid_opt(char *arg);
int				get_opt(char **args);
void			echo(char **args);
void			ft_exit(char **params, t_mshell *mini);
void			free_mini_simul_cd(t_mshell *mini);
int				is_alpha_num(char c);
int				char_is_digit(char c);
int				valid_param(char *param);
int				simul_ft_unset(char **params);

//**************************cmd_list_utils.c********************************

int				cmd_list_size(t_command *lst);
int				cmd_lst_pos(t_command *lst, t_command *current);

//**************************exec.c********************************

void			ft_exe(t_mshell *mini);
void			delete_hd(t_command *command);
void			exec_cmd(t_mshell *mini);
void			exe_child_daron(t_mshell *mini, t_command *cur, int i, int pid);
void			get_last_retour_builtin(t_command *lst, t_mshell *mini);
void			execute(t_mshell *mini, t_command *current, int i);
void			ft_dup(t_mshell *mini, t_command *current, int i);
void			dup_redir_in_out(t_command *current, t_mshell *mini);
void			error(char *str, t_mshell *mini, int erreur);
void			exe_child_daron(t_mshell *mini, t_command *cur, int i, int pid);
void			execute(t_mshell *mini, t_command *current, int i);
void			end_fonction(t_mshell *mini);
int				exit_if_pas_de_commande(t_mshell *mini);

//**************************exec_utils2.c********************************

void			exit_if_builtin_only(t_mshell *mini, t_command *current);
void			close_pipe_n_wait(int *pipe_fd, int *pids);
void			close_pipe_n_wait(int *pipe_fd, int *pids);
int				*set_pipe(t_command *command);

//**************************exec_utils.c********************************

void			exe_builtins(char **params, t_env **env, t_mshell *mini);
int				simul_exe_builtins(char **params, t_mshell *mini);
int				is_builtins(char **params);
int				lst_env_size(t_env *env);
void			exe_builtins(char **params, t_env **env, t_mshell *mini);
int				simul_exe_builtins(char **params, t_mshell *mini);
int				is_builtins(char **params);
char			**env_to_tab(t_env *env);

//**************************free.c********************************

void			free_redir_in(t_redir_in *lst);
void			free_redir_out(t_redir_out *lst);
void			free_mini(t_mshell *mini);

//**************************heredoc.c********************************

void			replace_heredoc(t_redir_in *to_change, char *hd_name);
char			*get_hd_name(void);
void			find_replace_hd(t_redir_in *lst);
void			heredoc(t_mshell *mini, char *name, char *stop);
int				launch_hd(t_mshell *mini);
void			replace_heredoc(t_redir_in *to_change, char *hd_name);
char			*get_hd_name(void);
void			heredoc(t_mshell *mini, char *name, char *stop);
void			jsuisbourre(t_command *current, t_redir_in *temp, \
													t_mshell *mini);

//**************************redir_in.c********************************

void			add_back_redir_in(t_redir_in **lst, char *file_name, int bol);
void			add_back_redir_in(t_redir_in **lst, char *file_name, int bol);
t_redir_in		*parse_redir_in(char **command);
void			add_back_redir_in(t_redir_in **lst, char *file_name, int bol);
char			**command_clear_all_in(char **command);
char			**command_clear_one_in(char **command);
t_redir_in		*parse_redir_in(char **command);

//**************************redir_out.c********************************

t_redir_out		*parse_redir_out(char **command);
char			**ft_dup_tab(char **command);
char			**command_clear_all_out(char **command);
char			**command_clear_one(char **command);
void			add_back_redir_out(t_redir_out **lst, char *file_name, int bol);
t_redir_out		*parse_redir_out(char **command);
int				simul_cd(char **params, t_mshell *mini);
int				simul_ft_exit(char **params, t_mshell *mini);
int				lataille(char **command);

//**************************utils.c********************************

void			free_tab(char **tab);
char			**create_paths(t_env *env);
void			free_tab(char **tab);
char			*ft_strjoin_f(char *s1, char const *s2);
char			*find_path(t_env *env, char **cmd_params);
char			*check_absolute_path(char *path);

//************************dup-redir_in_out.c*************************
void			ft_dup(t_mshell *mini, t_command *current, int i);
int				make_redir_out(t_command *command, t_mshell *mini);
int				make_redir_in(t_command *command, t_mshell *mini);
void			error(char *str, t_mshell *mini, int erreur);

//*************************decla.h*********************************
int				decla(int *i, int *j, int *found, char **command);
int				decla_v2(int *i, int *j, int *found, char **command);
void			init_things_to_save_two_little_lines_for_norme(int *i, \
												int *j, int *found);

#endif