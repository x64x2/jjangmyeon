#!/usr/bin/perl -w

#   jjangmyeon is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.

#   jjangmyeon is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.



#Call of the standard modules
use lib '/usr/share/jjangmyeon';
use LWP::UserAgent;
use strict;
use Switch 'Perl6';
use Term::ANSIColor qw(:constants);
use Getopt::Long;
$Term::ANSIColor::AUTORESET = 1;
Getopt::Long::Configure( "bundling" );


#Call of the program's modules
use Getinfos;
use Getdetails;
use Torrent;
use Tools;
use Configfile;

#Variables declaration

#Hash table to sort preferences in arguments
my %table_sort = ( "date" => "3",
	"se" => "7",
	"le" => "9",
	"user" => "12",
	"size" => "5");

my ($retour, $inc, $research, $sort, $sort_pref) = (1, 0, '', '7');
my $prog = "jjangmyeon";
my $compact = read_filerc()->compact;

#Retrieve options gived in argument
GetOptions("sort=s" => \$sort_pref);

if(defined($sort_pref))
{
    $sort = $table_sort{$sort_pref};
}

#Get the research in arguments if exist
foreach(@ARGV)
{
    $research .= $_ . ' ';
}
while($retour){

    $retour = main($sort);
}
print CLEAR BLUE BOLD "New research ? (y/N)". BOLD YELLOW "\n==> ";
my $newr = get_input();
if($newr eq "y"){
    exec $prog;
	die "$prog not find in $ENV{PATH}";
}

sub testcompactup{
    my $return;
    if($compact){
	$return = BOLD YELLOW "Num|".BOLD GREEN "SE|".BOLD RED "LE|".WHITE "Genre|".CLEAR BLUE BOLD"Name|".DARK GREEN "Author";
    }
    else{
	$return = "Num\tSE\tLE\tGenre\tName\n"; 
    }
    return $return;
}

sub testcompactdet{
    my $torrent = shift;
    my $return;
    if($compact){
	$return = GREEN " $torrent->{SEEDERS}". RED " $torrent->{LEECHERS}".WHITE " $torrent->{GENRE}". CLEAR BLUE BOLD" $torrent->{NAME}". DARK GREEN " $torrent->{AUTHOR}";
    }
    else{
	$return = "\t$torrent->{SEEDERS}\t$torrent->{LEECHERS}\t$torrent->{GENRE}\t" . CLEAR BLUE BOLD . "$torrent->{NAME}\n" . "\t"x4 . RESET GREEN "ULed by $torrent->{AUTHOR}\n";
    }
    return $return;
}

#Main function 
sub main{

#Initiation variables
    my $compact = 1;
    my ($sort) = @_;
    my ($page, $i, $torrent) = ('',0, Torrent->new());
    my (@pages, @urls_torrents, @urls_details);
    my $up = testcompactup();

#Call of the function of config file
    read_filerc();

#Retrieve the research
    if(!($research)){
        print CLEAR BLUE BOLD "PIRATE SEARCH : ";
	$research = get_input();}

#Get the web page
    $page = page_dwnld($research, 1, '', $sort); 

#Split the pages in pieces
    @pages = split(/detName/, $page);
    ($torrent->{GENRE}) = ($pages[1] =~ m/\"More from this category\"\>(\w+)\</);
    print $up;

#Boucle to find names and links
    foreach(@pages)
    {

#If this line contains a torrent name
	if (($torrent->{NAME}) = ($_ =~ m/title=\"Details for (.*)\"/))
	{
	    $i += 1;

	    retrieve_infos($_, $torrent);

	    print BOLD YELLOW "\n$i";
#print "\t$torrent->{SEEDERS}\t$torrent->{LEECHERS}\t$torrent->{GENRE}\t" . CLEAR BLUE BOLD . "$torrent->{NAME}\n" . "\t"x4 . RESET GREEN "ULed by $torrent->{AUTHOR}\n";
	    print testcompactdet($torrent);
	    get_url_torrent(\@urls_torrents, $_, $i);
	    get_urls_details(\@urls_details, $_, $i);
	    get_genre_torrent($_, $torrent);
	}
    }

    if ($i == 0)
    {
	print "No torrents found\n";
	exit(1);
    }

#Call choices function
    if((choices(\@urls_torrents, \@urls_details)) and ($page)){
	return 1;
    }
    else
    {
	return 0;
    }

}

#Function for retrieving infos
sub retrieve_infos
{
    my ($line, $torrent) = @_;
    get_seeds_leechers_torrent($_, $torrent);
    get_author_torrent($_, $torrent);
}

#Function for choosing action after research
sub choices
{
    my $choice;
    my $urls_torrents = shift;
    my $urls_details = shift;

    print BOLD YELLOW "\n==>" . CLEAR BLUE BOLD " Details (m) | display and copy the magnet link (c) | next page (t) | page no. (p) | new search (s) ?\n" . BOLD YELLOW "==> ";

    $choice = <STDIN>;

    given ($choice) {
	when(/m/) {
	    if(display_details($urls_details, $urls_torrents))
	    {
		return 1;
	    }
	}
	when(/c/){
	    display_magnetic_link($urls_torrents,$urls_details);
	}
	when(/t/){
	    return 1;
	}
	when(/p/){
	    print BOLD YELLOW "\n==> " . CLEAR BLUE BOLD "Which page ?" . BOLD YELLOW "\n==> " . RESET;
	    $page_n = get_input();
	    print $page_n;
	    return 1;
	}
	when(/s/){
	    exec $prog;
		die "$prog not found in $ENV{PATH}";
	}
	default{
	    print "Not a possible choice, quit... ";
	    exit(0);
	}
    }
    return 0;
}

it(0);
	}
    }
    return 0;
}

