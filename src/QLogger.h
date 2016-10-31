/*
 *  QLogger.h
 *
 *  Created by Andrea Cuius
 *  The MIT License (MIT)
 *  Copyright (c) 2015 Nocte Studio Ltd.
 *
 *  www.nocte.co.uk
 *
 */

#pragma once
#include "cinder/app/App.h"


class QLogger {

public:

    static void init( ci::fs::path logsPath );
    static void log( const std::string msg, bool logTimestamp = true );

private:
    
    static std::string  getTodayFileName();
    static std::string  getTodayDateTime();
    static void         deleteOldLogFiles();
    
private:

    static ci::fs::path    mLogsPath;
    
};
