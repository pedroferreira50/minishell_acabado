/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:05:30 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/05 03:35:08 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	*line;
	int		result;

	(void)argc;
	(void)argv;
	shell = get_shell();
	init_shell(shell, envp);
	rl_catch_signals = 0;
	setup_signals();
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
}

/* int	main(int argc, char *argv[], char *envp[])
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
} */

