using System;
using System.IO;
using System.Xml.Serialization;
using Microsoft.Deployment.WindowsInstaller;

namespace SetPathProperties
{
    public class CustomActions
    {
        [CustomAction]
        public static ActionResult SetPathProperties(Session session)
        {
            session.Log("Begin SetPathProperties");

            // Check the required property has been set
            string definitionToUse = session["SETPATHPROPERTIESDEF"];

            if (String.IsNullOrEmpty(definitionToUse))
            {
                throw new Exception("InstallValidator: ERROR : SETPATHPROPERTIESDEF not set. Inform the developers of the problem.");
            }

            // Log the properties for debugging if necessary
            session.Log("LOG: SETPATHPROPERTIESDEF : " + definitionToUse);
            
            // Build the location of the definition resource
            // Open the resource for reading
            var xmlStream = System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(String.Format("SetPathProperties.PathDefinitions.{0}.xml", definitionToUse));
            if (xmlStream == null)
            {
                throw new Exception(String.Format("Matching path properties definition for {0} was not found. Inform the developers of the problem.", definitionToUse));
            }

            // Deserialize the definition xml
            var serializer = new XmlSerializer(typeof(PathSet));
            var pathset = serializer.Deserialize(xmlStream) as PathSet;

            if (pathset == null)
            {
                throw new Exception(String.Format("Failed to deserialize a path properties definition xml {0}. Inform the developers of the problem.", definitionToUse));
            }

            // Check the root path has been set
            var sourcePropertyName = pathset.RootProperty;
            var rootPath = session[sourcePropertyName] as string;
            
            if (String.IsNullOrEmpty(rootPath) || !Directory.Exists(rootPath))
            {
                session["SETPATHPROPERTIESRESULT"] = "FAIL";
                return ActionResult.Success;
            }

            // Set the properties
            foreach(var property in pathset.Properties)
            {
                var propertyValue = property.Value.Replace("$(RootProperty)", rootPath);
                session.Log("LOG: " + property.Name + " : " + propertyValue);
                session[property.Name] = propertyValue;
            }
            session["SETPATHPROPERTIESRESULT"] = "PASS";
            return ActionResult.Success;
        }
    }
}
