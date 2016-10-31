/*
 *  QLogger.cpp
 *
 *  Created by Andrea Cuius
 *  The MIT License (MIT)
 *  Copyright (c) 2015 Nocte Studio Ltd.
 *
 *  www.nocte.co.uk
 *
 */


#include "QLogger.h"

#include "cinder/app/App.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include "cinder/Utilities.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

#define	MAX_NUM_LOG_FILES	10


fs::path    QLogger::mLogsPath;


void QLogger::init( fs::path logsPath )
{
    mLogsPath = logsPath;

    // create Logs dir if it doesn't exist
    if ( !fs::exists(mLogsPath) )
        fs::create_directories(mLogsPath);
    
    log( "-----------------------------------------------------" );
    log( "=== QLOGGER INITIALISED! === " + getTodayDateTime() );
    log( "-----------------------------------------------------" );
    
    
    deleteOldLogFiles();
}
    
void QLogger::log( const string msg, bool logTimestamp )
{
    string      fileName = mLogsPath.generic_string(); fileName += getTodayFileName();

    ofstream    openFile( fileName.c_str(), std::ios::app );
    
    if ( logTimestamp )
        openFile << getTodayDateTime() << ": ";
    
    openFile << msg << endl;

    openFile.close();
    console() << msg << endl;
}


string QLogger::getTodayFileName()
{
    time_t rawtime;
    time ( &rawtime );
    struct tm * timeinfo = localtime( &rawtime );
    
    string filename = to_string( timeinfo->tm_mday ) + "-" + to_string( timeinfo->tm_mon + 1 ) + "-" + to_string( 1900 + timeinfo->tm_year ) + ".txt";
    
    return filename;
}


string QLogger::getTodayDateTime()
{
    char buffer [80];
    time_t rawtime;
    time ( &rawtime );
    struct tm * timeinfo = localtime( &rawtime );
    strftime (buffer,80,"%c",timeinfo);
    return (string)buffer;
}


void QLogger::deleteOldLogFiles()
{
    vector<fs::path> logFiles;
    
    // get all files
    boost::filesystem::directory_iterator end_itr;
    for ( boost::filesystem::directory_iterator itr( mLogsPath ); itr != end_itr; ++itr )
    {
        if ( itr->path().extension() == ".txt" )
            logFiles.push_back( itr->path() );
    }
    
    // sort files by date
    std::sort( logFiles.begin(), logFiles.end(), [](fs::path a, fs::path b) {
        
        vector<string> a_values, b_values;
        boost::split( a_values, a.filename().generic_string(), boost::is_any_of("-") );
        boost::split( b_values, b.filename().generic_string(), boost::is_any_of("-") );
        
        if ( a_values[2] != b_values[2] )        return  a_values[2] > b_values[2];
        if ( a_values[1] != b_values[1] )        return  a_values[1] > b_values[1];
        if ( a_values[0] != b_values[0] )        return  a_values[0] > b_values[0];
        
        return true;
    });
    
    for( size_t k=MAX_NUM_LOG_FILES; k < logFiles.size(); k++ )
        fs::remove( logFiles[k] );
}

