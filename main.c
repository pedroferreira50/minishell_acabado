/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:05:30 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 08:28:20 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_assignment_non_export(char *input, t_parse_result *parsed)
{
	(void)input;
	free_args(parsed->args, NULL);
	free(parsed->quote_types);
}

void	handle_command(char *input, t_shell *shell)
{
	t_parse_result	parsed;
	t_command_data	*data;
	char			**expanded_args;
	char			**filtered_args;
	int				pre_parse_exit_status;

	expanded_args = parse_and_expand_command(input, shell, &parsed);
	if (!expanded_args)
		return ;
	filtered_args = create_filtered_args(expanded_args);
	if (!filtered_args)
		return (cleanup_parse_data(&parsed, expanded_args, NULL));
	pre_parse_exit_status = shell->exit_status;
	data = prepare_command_data(filtered_args, shell);
	cleanup_parse_data(&parsed, expanded_args, filtered_args);
	if (!data)
		return ;
	if (pre_parse_exit_status == 0 && shell->exit_status == 1
		&& data->num_pipes == 0)
	{
		cleanup_command_data(data);
		return ;
	}
	execute_command_pipeline(data, shell);
}

int	process_input(char *input, t_shell *shell)
{
	int	i;

	if (g_signal == SIGINT)
	{
		shell->exit_status = 130;
		g_signal = 0;
		return (1);
	}
	if (input == NULL)
		return (0);
	if (input[0] == '\0')
		return (1);
	g_signal = 0;
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	add_history(input);
	if (is_var_assignment(input))
	{
		handle_var_assignment(input, shell);
		return (1);
	}
	return (2);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	*shell;
	char	*input;

	(void)argc;
	(void)argv;
	shell = get_shell();
	init_shell(shell, envp);
	rl_catch_signals = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (process_input(input, shell) == 0)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (input && input[0] != '\0' && process_input(input, shell) == 2)
		{
			handle_command(input, shell);
			if (input && input[0] != '\0')
				free(input);
		}
	}
	finalize_shell(shell);
	return (shell->exit_status);
}

/* int	main(int argc, char *argv[], char *envp[])
{
	t_shell	*shell;
	char	*input;
	char	*line;
	int		result;

	(void)argc;
	(void)argv;
	shell = get_shell();
	init_shell(shell, envp);
	rl_catch_signals = 0;
	set_signals_interactive();
	while (1)
	{
		if (isatty(fileno(stdin)))
			input = readline("minishell> ");
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
			{
				input = NULL;
			}
			else
			{
				input = ft_strtrim(line, "\n");
				free(line);
			}
		}
		result = process_input(input, shell);
		if (result == 0)
		{
			break ;
		}
		if (input && input[0] != '\0' && result == 2)
		{
			handle_command(input, shell);
		}
		if (input)
			free(input);
	}
	finalize_shell(shell);
	return (shell->exit_status);
} */
