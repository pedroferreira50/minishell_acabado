/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:15:44 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/05 01:28:21 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_parsed_data(t_parse_result *parsed, char **expanded_args)
{
	free_args(parsed->args, NULL);
	if (expanded_args)
		free_args(expanded_args, NULL);
	free(parsed->quote_types);
}

static int	count_expanded_args(char **expanded_args)
{
	int	count;

	count = 0;
	while (expanded_args[count])
		count++;
	return (count);
}

static char	**create_filtered_args(char **expanded_args, int count)
{
	char	**filtered_args;
	int		i;
	int		j;

	filtered_args = malloc(sizeof(char *) * (count + 1));
	if (!filtered_args)
		return (NULL);
	i = 0;
	j = 0;
	while (expanded_args[i])
	{
		filtered_args[j] = ft_strdup(expanded_args[i]);
		j++;
		i++;
	}
	filtered_args[j] = NULL;
	return (filtered_args);
}

static t_command_data	*init_command_data(void)
{
	t_command_data	*data;

	data = malloc(sizeof(t_command_data));
	if (!data)
		return (NULL);
	ft_memset(data, 0, sizeof(t_command_data));
	return (data);
}

static char	**process_parsed_command(t_parse_result *parsed, t_shell *shell,
		int *count)
{
	char	**expanded_args;

	expanded_args = expand_tokens(parsed->args, parsed->quote_types, shell);
	if (!expanded_args)
	{
		cleanup_parsed_data(parsed, NULL);
		return (NULL);
	}
	*count = count_expanded_args(expanded_args);
	if (*count == 0)
	{
		cleanup_parsed_data(parsed, expanded_args);
		return (NULL);
	}
	return (expanded_args);
}

static int	validate_and_execute(t_command_data *data, t_shell *shell,
		int pre_parse_exit_status)
{
	if (pre_parse_exit_status == 0 && shell->exit_status == 1
		&& data->num_pipes == 0)
		return (0);
	if (shell->exit_status != 2 && validate_command(data->commands, shell) == 0)
		return (0);
	execute_commands(data, shell);
	return (1);
}

static void	execute_with_data(t_shell *shell, t_parse_result *parsed,
		char **expanded_args, char **filtered_args)
{
	t_command_data	*data;

	data = init_command_data();
	if (!data)
	{
		cleanup_parsed_data(parsed, expanded_args);
		free_args(filtered_args, NULL);
		return ;
	}
	handle_command_execution(shell, parsed, data, filtered_args);
	cleanup_parsed_data(parsed, expanded_args);
	free_args(filtered_args, NULL);
}

void	handle_command(char *input, t_shell *shell)
{
	t_parse_result	parsed;
	char			**expanded_args;
	char			**filtered_args;
	int				count;

	parsed = parse_command(input, shell);
	if (!parsed.args)
		return ;
	expanded_args = process_parsed_command(&parsed, shell, &count);
	if (!expanded_args)
		return ;
	filtered_args = create_filtered_args(expanded_args, count);
	if (!filtered_args)
	{
		cleanup_parsed_data(&parsed, expanded_args);
		return ;
	}
	execute_with_data(shell, &parsed, expanded_args, filtered_args);
}

void	handle_command_execution(t_shell *shell, t_parse_result *parsed,
		t_command_data *data, char **filtered_args)
{
	int	pre_parse_exit_status;
	int	count;

	(void)parsed;
	count = 0;
	while (filtered_args[count])
		count++;
	pre_parse_exit_status = shell->exit_status;
	parse_input(filtered_args, count, data, shell);
	if (!validate_and_execute(data, shell, pre_parse_exit_status))
	{
		free_command_data(data);
		free(data);
		return ;
	}
	free_command_data(data);
	free(data);
}
