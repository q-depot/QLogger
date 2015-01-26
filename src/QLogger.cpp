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

#include "cinder/app/AppBasic.h"
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

fs::path    QLogger::mLogsPath;


void QLogger::init( fs::path logsPath )
{
    mLogsPath = logsPath;

    // create Logs dir if it doesn't exist
    createDirectories(mLogsPath);
        
    log( "-----------------------------------------------------" );
    log( "=== QLOGGER INITIALISED! === " + getTodayDateTime() );
    log( "-----------------------------------------------------" );
        
    // delete all the files older than KEEP_FOR_N_DAYS
    boost::filesystem::directory_iterator end_itr;
    for ( boost::filesystem::directory_iterator itr( mLogsPath ); itr != end_itr; ++itr )
    {
        if ( itr->path().extension() != ".txt" )
            continue;

        std::vector<std::string> fileNameStrs;
        std::string filename = itr->path().filename().generic_string();
        boost::split(fileNameStrs, filename, boost::is_any_of("."));
        boost::gregorian::date logDate(boost::gregorian::from_string(fileNameStrs[0]));
            
        if ( logDate <  boost::gregorian::day_clock::local_day() - boost::gregorian::days(KEEP_LOGS_FOR_N_DAYS) )
        {
            deleteFile( mLogsPath.generic_string() + filename );
            log( "delete log file: " + filename );
            //	ci::deleteFile( APP_LOGS_PATH + itr->leaf() );
            //	log( "delete log file: " + itr->leaf() );
        }
    }
        
}
    
void QLogger::log( const string msg )
{
    string      fileName = mLogsPath.generic_string(); fileName += getTodayFileName();

    ofstream    openFile( fileName.c_str(), std::ios::app );
    openFile << getTodayDateTime() << ": " << msg << endl;
    openFile.close();
    console() << msg << endl;
}


string QLogger::getTodayFileName()
{
    string str;
    str += to_iso_extended_string( boost::gregorian::day_clock::local_day()) + ".txt";
    return  str;
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

