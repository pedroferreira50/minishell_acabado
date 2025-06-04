/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:42:54 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:42:59 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_tilde(char *token, t_shell *shell)
{
	char	*home;
	char	*result;
	size_t	home_len;
	size_t	token_len;

	if (!token || token[0] != '~')
		return (ft_strdup(token));
	if (token[1] != '\0' && token[1] != '/')
		return (ft_strdup(token));
	home = get_var_value_helper("HOME", shell->vars, shell->envp);
	if (!home || !*home)
		return (ft_strdup(token));
	home_len = ft_strlen(home);
	token_len = ft_strlen(token);
	result = malloc(home_len + token_len);
	if (!result)
		return (ft_strdup(token));
	ft_strcpy(result, home);
	if (token[1] == '/')
		ft_strcat(result, &token[1]);
	return (result);
}

void	build_command_data(char **args, int argc, t_command_data *data,
	t_shell *shell)
{
	if (data->arguments == NULL)
	{
		data->arguments = malloc(sizeof(char **) * 2);
		if (!data->arguments)
			return ;
		data->arguments[0] = args;
		data->arguments[1] = NULL;
		data->num_commands = 1;
	}
	parse_input(args, argc, data, shell);
}

static void	execute_and_free(t_command_data *data, t_shell *shell)
{
	if (data->arguments != NULL)
	{
		execute_commands(data, shell);
		free(data->arguments);
	}
}

void	expand_and_validate(char **tokens, char *quote_types, t_shell *shell)
{
	char			**expanded;
	t_command_data	data;

	if (!tokens || !tokens[0])
	{
		if (shell->exit_status != 2)
			shell->exit_status = 0;
		return ;
	}
	expanded = expand_tokens(tokens, quote_types, shell);
	if (!expanded)
	{
		shell->exit_status = 1;
		return ;
	}
	if (!validate_command(expanded, shell))
	{
		free_args(expanded, NULL);
		return ;
	}
	build_command_data(expanded, count_args(expanded), &data, shell);
	execute_and_free(&data, shell);
}
