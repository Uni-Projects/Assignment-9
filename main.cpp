/*********************************************************************
*   Example framework for assignment 9 IPC031.
*   Content:
*   - the type definitions for the music database for *arrays* (convert this yourself to vector, see part 1)
*   - ordering relations on Track;
*   - reading Tracks.txt file;
*   - functions and data structures from lecture #9 IPC031:
*   - sorting algorithms insertion_sort, selection_sort, bubble_sort
*     (based on *arrays*, convert this yourself to vector)
*   - main that reads Tracks.txt and calls a sorting algorithm.
**********************************************************************/
//Assignment 9
//Paolo Scattolin s1023775
//Johan Urban s1024726
#define NDEBUG
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string.h>

using namespace std;


/*********************************************************************
*   type definitions and global array songs:
**********************************************************************/
struct Length
{
	int minutes;							// #minutes (0..)
	int seconds;							// #seconds (0..59)
};
struct Track
{
	string artist;                          // artist name
	string cd;                              // cd title
	int    year;                            // year of appearance
	int    track_no;						// track number
	string title;                           // track title
	string tags;                            // track tags (separated by ,)
	Length time;							// track length
	string country;                         // countr(y/ies) of artist (separated by ,)
};
int iteration = 0;
const int MAX_NO_OF_SONGS = 6000;           // NOTE: only required for *array*
vector<Track> songs ;              // NOTE: this must become vector<Track>

/************************************************************************
*   Ordering relations op Track:
*      define < and == for Length and Track yourself, the other
*      ordering relations (>, <=, >=, >) follow from these automatically.
************************************************************************/
bool operator<(const Length& a, const Length& b)
{
    iteration++;
    if (a.minutes == b.minutes)
        return a.seconds < b.seconds;
    else return a.minutes < b.minutes;

}

bool operator==(const Length& a, const Length& b)
{
    iteration++;
    return (a.minutes == b.minutes && a.seconds == b.seconds) ;   /* implement a correct == ordering on Track values */
}

bool operator>(const Length& a, const Length& b)
{
	return b < a ;
}

bool operator<=(const Length& a, const Length& b)
{
	return !(b < a) ;
}

bool operator>=(const Length& a, const Length& b)
{
	return b <= a ;
}

bool operator<(const Track& a, const Track& b)
{
    iteration++;
    if(a.artist == b.artist)
    {
         if(a.cd == b.cd)
         {
             if(a.year == b.year)
             {
                return a.track_no < b.track_no;
             }else return a.year < b.year;

         }else return a.cd < b.cd;

    }else return a.artist < b.artist ;
    //return a.time < b.time ;
}

bool operator==(const Track& a, const Track& b)
{
    iteration++;
    return (a.artist == b.artist && a.cd == b.cd && a.year == b.year && a.track_no == b.track_no) ;
    //return a.time == b.time;
}

bool operator>(const Track& a, const Track& b)
{
	return b < a ;
}

bool operator<=(const Track& a, const Track& b)
{
	return !(b < a) ;
}

bool operator>=(const Track& a, const Track& b)
{
	return b <= a ;
}


/************************************************************************
*   Reading Tracks.txt file:
************************************************************************/
istream& operator>> (istream& in, Length& length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    a value for length has been obtained from in: first minutes, followed by ':', followed by seconds.
*/
    char colon ;
    in >> length.minutes >> colon >> length.seconds ;
    return in ;
}

ostream& operator<< (ostream& out, const Length length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    length is sent to out as: minutes, ':', seconds (at least 2 digits)
*/
    out << length.minutes << ':';
	if (length.seconds < 10)
		out << '0';
	out << length.seconds;
	return out ;
}

void skip_to_next_newline (istream& infile)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including the next newline character.
*/
    string remains ;
    getline (infile, remains) ;
}

istream& operator>> (istream& infile, Track& track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including all members of one Track entry, including the last separating newline.
    These members are stored in track.
*/
    getline(infile,track.artist);       // artist
    getline(infile,track.cd);           // cd
    infile >> track.year ;              // year
    skip_to_next_newline(infile) ;
    infile >> track.track_no ;          // track number
    skip_to_next_newline(infile) ;
    getline(infile,track.title);        // track title
    getline(infile,track.tags);         // track tags
    infile >> track.time ;              // track time
    skip_to_next_newline(infile) ;
    getline(infile,track.country);      // country
    skip_to_next_newline(infile) ;
    return infile ;
}

// not necessary for assignment (because of show_track function), but included anyway because of symmetry:
ostream& operator<< (ostream& out, const Track track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    out has emitted all members of track, including the last separating newline.
*/
    out << track.artist   << endl ;
    out << track.cd       << endl ;
    out << track.year     << endl ;
    out << track.track_no << endl ;
    out << track.title    << endl ;
    out << track.tags     << endl ;
    out << track.time     << endl ;
    out << track.country  << endl ;
    out << endl ;
    return out ;
}

// NOTE: this version uses *array* implementation, convert to vector yourself
int read_songs (ifstream& infile, vector <Track>& songs)
{// pre-condition:
    assert (infile.is_open ()) ;
/*  post-condition:
    All tracks in infile have been read and stored in the same order in songs.
    The result is the number of tracks that have been read.
*/
    int i = 0 ;
    do {
        Track song ;
        infile >> song ;
        if (infile)
        {
            songs.push_back(song);
            i++;
        }
    }
    while (infile && i < MAX_NO_OF_SONGS);

    return i ;
}

int read_file (string filename)
{// pre-condition:
    assert (songs.size() == 0) ;
/*  post-condition:
    If the result is -1, then no file could be opened, and songs is unchanged.
    Otherwise, the result is zero or positive, and songs contains all the found tracks in the
    file that corresponds with filename.
*/
    ifstream songsDBS (filename.c_str());
    if (!songsDBS)
    {
        cout << "Could not open '" << filename << "'." << endl;
        return -1;
    }
    cout << "Reading '" << filename << "'." << endl;
	const int NO_OF_SONGS = read_songs (songsDBS, songs);
	songsDBS.close();
	cout << "Read " << NO_OF_SONGS << " tracks." << endl;
	return NO_OF_SONGS;
}

// NOTE: this version uses *array* implementation, convert to vector yourself
void show_all_tracks (vector <Track> songs, int no_of_songs)
{// pre-condition:
    assert (no_of_songs < MAX_NO_OF_SONGS) ;
/*  post-condition:
    songs[0] ... songs[no_of_songs-1] have been displayed to cout.
*/
    for (int i = 0 ; i < no_of_songs ; i++)
    {
        cout << i+1 <<". "<< songs[i] << endl ;
    }
}

/************************************************************************
*   functions and data structures from lecture #9 IPC031:
************************************************************************/
struct Slice
{
	int from ; 	// 0    <= from
	int to ; 	// from <= to
} ;

Slice mkSlice (int from, int to)
{//	pre-condition:
	assert (0 <= from && from <= to);
/*  post-condition:
    result is the slice formed by from and to.
*/	Slice s = { from, to } ;
	return s ;
}

bool valid_slice (Slice s)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    result is true only if s.from is not negative, and s.from is not larger than s.to.
*/
	return 0 <= s.from && s.from <= s.to ;
}

void print (int index)
{
    int stars = iteration/100000;
    cout << "After " << index << " element sorted:";

    while (stars > 0 )
    {
        cout <<"*";
        stars--;
    }
    cout<<"."<< endl;
}

typedef vector<Track> El ;                // NOTE: for the assignment you need to sort Track values

bool is_sorted ( El& data, Slice s)
{//	pre-condition:
	assert (valid_slice(s)) ;	// ...and s.to < size of data
//	post-condition:
//	result is true if	data[s.from]   <= data[s.from+1]
//						data[s.from+1] <= data[s.from+2]
//						...
//						data[s.to-1]   <= data[s.to]
	bool sorted = true ;
	for (int i = s.from; i < s.to && sorted; i++)
		if (data[i] > data[i+1])
			sorted = false ;
	return sorted ;
}

int find_position ( El& data, Slice s, Track y )
{//	pre-condition:
	assert (valid_slice(s) && is_sorted(data,s)) ;    // ...and s.to < size of data
//	post-condition: s.from <= result <= s.to+1
	for ( int i = s.from ; i <= s.to ; i++ )
		if ( y <= data [i] )
			return i ;
	return s.to+1;
}

void shift_right ( El& data, Slice s )
{//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (data) - 1
//	post-condition:  data[s.from+1]	= 	old data[s.from]
//			    	 data[s.from+2]	= 	old data[s.from+1]
//						...
//			    	 data[s.to+1]		= 	old data[s.to]
	for ( int i = s.to+1 ; i > s.from ; i-- )
		data [i]  = data [i-1] ;
}

void insert ( El& data, int& length, Track y )
{// pre-condition:
    Slice s = mkSlice(0,length-1) ;
    assert (length >= 0 && is_sorted (data, s)) ;
/*  post-condition:
    value y is inserted in data[0] ... data[length-1] at the right ordering position,
    and length is increased by one.
*/
	const int POS = find_position (data, mkSlice (0, length-1), y) ;
	if (POS < length)
		shift_right ( data, mkSlice (POS, length-1) ) ;
	data [POS] = y ;
	length++;
}

void swap (El& data, int  i, int  j )
{//	pre-condition:
	assert ( i >= 0 && j >= 0 ) ;	// ... and i < size of data
						            // ... and j < size of data
// Post-condition: data[i] = old data[j] and data[j] = old data[i]
	const Track HELP = data [i];
	data [i] = data [j] ;
	data [j] = HELP;
}

//	array based insertion sort:
void insertion_sort ( El& data, int length )
{
	int sorted = 1 ;
    while ( sorted < length )
    {
        insert ( data, sorted, data[sorted] ) ;
    }
}


//	array based selection sort:
int smallest_value_at ( El& data, Slice s )
{//	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (s)
//	Post-condition: s.from <= result <= s.to and data[result] is
//	the minimum value of data[s.from] .. data[s.to]
	int smallest_at = s.from ;
	for ( int index = s.from+1 ; index <= s.to ; index++ )
		if ( data [index] < data [smallest_at] )
			smallest_at = index ;
	return smallest_at ;
}

void selection_sort ( El& data, int length )
{
    for ( int unsorted = 0 ; unsorted < length ; unsorted++ )
	{
        const int k = smallest_value_at (data, mkSlice (unsorted, length-1));
	    swap ( data, unsorted, k ) ;
	}
}

//	array based bubble sort:
bool bubble ( El& data , Slice s )
{//	pre-condition:
	assert (valid_slice(s));	// ... and s.to < size(data)
//	Post-condition:
//	maximum of data[s.from]..data[s.to] is at data[s.to]
//	if result is true then sorted( data, s )
	bool is_sorted = true ;
	for ( int i = s.from ; i < s.to ; i++)
	    if ( data [i] > data [i+1])
		{	swap ( data, i, i+1 ) ;
			is_sorted = false ;
		}
	return is_sorted ;
}

void bubble_sort ( El& data, int length )
{
    while ( !bubble ( data, mkSlice (0, length-1 ) ) )
        length-- ;
}


/************************************************************************
*   the main structure of the program:
*       - read file
*       - sort data with insertion_sort / selection_sort / bubble_sort
*              and 'normal' order of Tracks
*              and increasing track length
************************************************************************/
enum SortingMethod {InsertionSort,SelectionSort,BubbleSort,NoOfSortingMethods};
string methods [] = {"insertion", "selection", "bubble"} ;

SortingMethod get_sorting_method ()
{
    cout << "What sorting method do you want to use? " << endl ;
    for (int m = 0; m < NoOfSortingMethods; m++)
        cout << m+1 << ": " << methods[m] << " sort" << endl ;
    int choice ;
    cin >> choice ;
    choice = max(1, min(choice, (int)NoOfSortingMethods)) ;
    return static_cast<SortingMethod>(choice-1) ;
}

int main()
{
	const int NO_OF_SONGS = read_file ("Tracks.txt");
	if (NO_OF_SONGS < 0)
	{
        cout << "Reading file failed. Program terminates." << endl;
	    return NO_OF_SONGS;
    }

    SortingMethod m = get_sorting_method () ;
    cout << "Sorting tracks with " << methods[m] << " sort" << endl;

    for (int i = 99; i  <= NO_OF_SONGS; i += 100)
    {
        vector<Track> subvector(songs.begin(), songs.begin() + i);
        switch (m)
        {
           case InsertionSort: insertion_sort(subvector,i) ; break ;
           case SelectionSort: selection_sort(subvector,i) ; break ;
           case BubbleSort:    bubble_sort   (subvector,i) ; break ;
           default: cout << "Huh?" << endl ;
        }
        //cout << "Tracks sorted." << endl;

        //show_all_tracks (songs,NO_OF_SONGS) ;
        cout << iteration << endl;
        print(i);
        iteration = 0 ;
        if (i == 5799)
        i=NO_OF_SONGS-100;
    }

	return 0;
}
