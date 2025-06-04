/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:06:32 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:23:26 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_cd_error(t_shell *shell, char *old_pwd, char *path)
{
	print_error_command("cd", path, 1, shell);
	ft_putstr_fd(": No such file or directory\n", 2);
	shell->exit_status = 1;
	free(old_pwd);
	return (1);
}

static int	handle_cd_too_many_arguments(t_shell *shell)
{
	print_error_simple("cd: too many arguments", 1, shell);
	return (1);
}

int	ft_cd(char **args, int *i, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	if (count_args(args) > 2)
		return (handle_cd_too_many_arguments(shell));
	old_pwd = getcwd(NULL, 0);
	if (determine_cd_path(args, &path, shell) != 0)
		return (1);
	if (chdir(path) != 0)
		return (handle_cd_error(shell, old_pwd, path));
	if (update_pwd_vars(old_pwd, shell) != 0)
		return (1);
	free(old_pwd);
	if (i)
		(*i)++;
	shell->exit_status = 0;
	return (1);
}
