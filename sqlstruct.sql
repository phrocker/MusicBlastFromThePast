# Generation Time: Jul 20, 2004 at 10:50 AM
# Server version: 4.0.18
# PHP Version: 4.3.4
# 
# Database : `test`
# 

# --------------------------------------------------------

#
# Table structure for table `Permissions`
#
# Creation: Jul 20, 2004 at 10:46 AM
# Last update: Jul 20, 2004 at 10:46 AM
#

CREATE TABLE `Permissions` (
  `id` varchar(25) NOT NULL default '',
  `name` varchar(10) NOT NULL default '',
  `value` char(1) NOT NULL default '',
  `site` varchar(100) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `Permissions`
#


# --------------------------------------------------------

#
# Table structure for table `hidText`
#
# Creation: Jul 20, 2004 at 08:39 AM
# Last update: Jul 20, 2004 at 10:18 AM
#

CREATE TABLE `hidText` (
  `rid` varchar(25) NOT NULL default '',
  `number` smallint(3) NOT NULL default '0',
  `text` text NOT NULL
) TYPE=MyISAM;

#
# Dumping data for table `hidText`
#

INSERT INTO `hidText` (`rid`, `number`, `text`) VALUES ('trhriVQWqRXynwqGkdBo', 0, '-115 99 -86 82 -19 -22 -67 -51 110 -88 43 90 27 -34 104 -53');

# --------------------------------------------------------

#
# Table structure for table `logs`
#
# Creation: Jul 20, 2004 at 08:43 AM
# Last update: Jul 20, 2004 at 10:18 AM
# Last check: Jul 20, 2004 at 09:07 AM
#

CREATE TABLE `logs` (
  `accountid` varchar(50) NOT NULL default '',
  `messid` varchar(50) NOT NULL default '',
  `gid` int(11) NOT NULL default '', 
  `title` varchar(255) NOT NULL default '',
  `timestamp` varchar(75) NOT NULL default '',
  `caption` text NOT NULL,
  `rid` varchar(20) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `logs`
#

INSERT INTO `logs` (`accountid`, `messid`, `title`, `timestamp`, `caption`, `rid`) VALUES ('', '', 'Marc', '', '112 -55 72 -117 -117 -71 -73 -115 -126 -77 123 37 -128 124 115 117 -105 58 70 -33 52 -123 -108 -62 96 -84 103 -108 22 -100 112 81 119 19 -49 50 98 -96 83 63 -75 47 -27 -73 -80 -98 32 77', 'trhriVQWqRXynwqGkdBo');

# --------------------------------------------------------

#
# Table structure for table `sessions`
#
# Creation: Jul 19, 2004 at 08:14 PM
# Last update: Jul 19, 2004 at 08:14 PM
#

CREATE TABLE `sessions` (
  `log_id` varchar(100) NOT NULL default '',
  `account_id` varchar(100) NOT NULL default '',
  `ip` varchar(15) NOT NULL default '',
  `connect_date` date NOT NULL default '0000-00-00',
  `connect_time` time NOT NULL default '00:00:00',
  `hashid` tinytext NOT NULL
) TYPE=MyISAM;

#
# Dumping data for table `sessions`
#


# --------------------------------------------------------

#
# Table structure for table `testing`
#
# Creation: Jul 17, 2004 at 02:35 PM
# Last update: Jul 18, 2004 at 03:46 PM
#

CREATE TABLE `testing` (
  `firstname` varchar(50) NOT NULL default '',
  `lastname` varchar(255) NOT NULL default '',
  `middle` varchar(50) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `testing`
#


# --------------------------------------------------------

#
# Table structure for table `users`
#
# Creation: Jul 18, 2004 at 04:36 PM
# Last update: Jul 18, 2004 at 04:36 PM
#

CREATE TABLE `users` (
  `gid` int(11) NOT NULL default '',
  `username` varchar(75) NOT NULL default '',
  `password` varchar(15) NOT NULL default '',
  `site` varchar(50) NOT NULL default '',
  `time` datetime NOT NULL default '0000-00-00 00:00:00'
) TYPE=MyISAM;

#
# Dumping data for table `users`
#

INSERT INTO `users` (`username`, `password`, `site`, `time`) VALUES ('marcParisi', 'password', '', '0000-00-00 00:00:00');

# --------------------------------------------------------

#
# Table structure for table `workgroups`
#
# Creation: Jul 20, 2004 at 10:49 AM
# Last update: Jul 20, 2004 at 10:49 AM
#

CREATE TABLE `workgroups` (
  `gid` int(11) NOT NULL auto_increment,
  `name` varchar(125) NOT NULL default '',
  `status` varchar(8) NOT NULL default '',
  KEY `gid` (`gid`)
) TYPE=MyISAM AUTO_INCREMENT=2 ;

#
# Dumping data for table `workgroups`
#

INSERT INTO `workgroups` (`gid`, `name`, `status`) VALUES (1, 'INTERN', 'ACTIVE');
    
