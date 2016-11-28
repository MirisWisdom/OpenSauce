using System;
using System.IO;
using System.Xml.Serialization;
using System.Xml;
using System.Windows.Forms;
using System.ComponentModel;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	static class Util
	{
		public static void SerializeObjectXML(object instance, string output_file)
		{
			XmlSerializer serializer = new XmlSerializer(instance.GetType());

			XmlWriterSettings xml_settings = new XmlWriterSettings();
			xml_settings.Indent = true;

			using (XmlWriter xml_writer = XmlWriter.Create(output_file, xml_settings))
				serializer.Serialize(xml_writer, instance);
		}

		public static object DeserializeObjectXML(Type object_type, string input_file)
		{
			XmlSerializer serializer = new XmlSerializer(object_type);

			XmlReaderSettings xml_settings = new XmlReaderSettings();

			using (XmlReader xml_reader = XmlReader.Create(input_file, xml_settings))
				return serializer.Deserialize(xml_reader);
		}

		public static string GetDirectoryDialog(string start_directory)
		{
			FolderBrowserDialog folder_browser = new FolderBrowserDialog();

			if (Directory.Exists(start_directory))
				folder_browser.SelectedPath = start_directory;

			DialogResult result = folder_browser.ShowDialog();

			if (result == DialogResult.Cancel)
				return "";

			return folder_browser.SelectedPath;
		}

		public static string GetSingleFileOpenDialog(string start_directory, string filter, string title)
		{
			OpenFileDialog file_dialog = new OpenFileDialog();
			file_dialog.Filter = filter;
			file_dialog.Multiselect = false;
			file_dialog.Title = title;

			if (Directory.Exists(start_directory))
				file_dialog.InitialDirectory = start_directory;

			DialogResult result = file_dialog.ShowDialog();

			if (result == DialogResult.Cancel)
				return "";

			return file_dialog.FileName;
		}

		public static string GetSingleFileSaveDialog(string start_directory, string filter, string title)
		{
			SaveFileDialog file_dialog = new SaveFileDialog();
			file_dialog.Filter = filter;
			file_dialog.Title = title;

			if (Directory.Exists(start_directory))
				file_dialog.InitialDirectory = start_directory;

			DialogResult result = file_dialog.ShowDialog();

			if (result == DialogResult.Cancel)
				return "";

			return file_dialog.FileName;
		}

		public static bool BindingListFind<T>(BindingList<T> list, Predicate<T> comparator)
		{
			foreach (var value in list)
				if (comparator(value))
					return true;
			return false;
		}
	}
}
