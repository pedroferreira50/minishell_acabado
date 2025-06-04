/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:57:03 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:57:04 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_empty_command(char **args, t_shell *shell)
{
	if (!args || !args[0])
	{
		shell->exit_status = 0;
		return (0);
	}
	if (args[0] && !*args[0])
	{
		shell->exit_status = 0;
		return (0);
	}
	return (1);
}

static int	validate_directory_path(char *path, t_shell *shell)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		print_error_command(path, "No such file or directory", 127, shell);
		return (0);
	}
	if (S_ISDIR(st.st_mode))
	{
		print_error_command(path, "Is a directory", 126, shell);
		return (0);
	}
	if (S_ISREG(st.st_mode) && access(path, X_OK) != 0)
	{
		print_error_command(path, "Permission denied", 126, shell);
		return (0);
	}
	return (1);
}

static int	validate_path_command(char *command, t_shell *shell)
{
	if (strchr(command, '/') != NULL)
	{
		return (validate_directory_path(command, shell));
	}
	return (1);
}

int	validate_command(char **args, t_shell *shell)
{
	if (!validate_empty_command(args, shell))
		return (0);
	if (check_builtin(args[0]))
		return (1);
	return (validate_path_command(args[0], shell));
}
