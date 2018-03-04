using System.Security.Principal;

Task("TestIsElevatedWindows")
	.WithCriteria(IsRunningOnWindows)
	.Does(() =>
	{
		var identity = WindowsIdentity.GetCurrent();
		var principal = new WindowsPrincipal(identity);
		if(!principal.IsInRole(WindowsBuiltInRole.Administrator))
		{
			throw new UnauthorizedAccessException("The script is not running with administrator permissions");
		}
	});
