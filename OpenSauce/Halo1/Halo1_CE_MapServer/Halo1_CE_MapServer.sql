-- phpMyAdmin SQL Dump
-- version 3.4.10.1deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 27, 2012 at 04:31 PM
-- Server version: 5.5.28
-- PHP Version: 5.3.10-1ubuntu3.4

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Halo1_CE_MapServer`
--
CREATE DATABASE `Halo1_CE_MapServer` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `Halo1_CE_MapServer`;

-- --------------------------------------------------------

--
-- Table structure for table `map_list`
--

CREATE TABLE IF NOT EXISTS `map_list` (
  `file_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `map_name` varchar(255) NOT NULL,
  `map_extension` varchar(5) NOT NULL,
  `map_compression_stage` tinyint(1) NOT NULL DEFAULT '0',
  `map_compression_type` tinyint(1) NOT NULL,
  `map_md5` varchar(32) NOT NULL,
  `map_uncompressed_size` int(11) unsigned NOT NULL,
  `map_compressed_size` int(11) unsigned NOT NULL,
  `map_parts_path` varchar(255) NOT NULL,
  `map_part_count` smallint(1) NOT NULL,
  PRIMARY KEY (`file_id`),
  UNIQUE KEY `file_name` (`map_name`),
  KEY `extension` (`map_extension`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `map_part_list`
--

CREATE TABLE IF NOT EXISTS `map_part_list` (
  `map_file_id` int(11) NOT NULL,
  `part_id` int(11) NOT NULL,
  `part_name` varchar(255) NOT NULL,
  `part_md5` varchar(32) NOT NULL,
  `part_size` int(11) unsigned NOT NULL,
  KEY `map_chunk_id` (`part_id`),
  KEY `file_id` (`map_file_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `map_server_users`
--

CREATE TABLE IF NOT EXISTS `map_server_users` (
  `index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `password_hash` varchar(255) NOT NULL,
  `user_control_permissions` int(11) unsigned DEFAULT '0',
  `map_database_permissions` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `map_server_users`
--

INSERT INTO `map_server_users` (`index`, `username`, `password_hash`, `user_control_permissions`, `map_database_permissions`) VALUES
(0, 'admin', '$P$JhEVpqVtzz.OYPx/w7jjuDf62HtAzC/', 7, 7);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
