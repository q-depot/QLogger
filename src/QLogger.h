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
#include "cinder/app/AppBasic.h"

#define	KEEP_LOGS_FOR_N_DAYS	30


class QLogger {

public:

    static void init( ci::fs::path logsPath );
    static void log(const std::string msg);

private:
    
    static std::string getTodayFileName();
    static std::string getTodayDateTime();

private:

    static ci::fs::path    mLogsPath;
    
};
