#include "DataService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

DataService::DataService(string _date, string _dataPath, map<string, StockDataInfo> _targetStocksDataInfo, vector<string> _targetStocks) {
    date = _date;
    dataPath = _dataPath;
    targetStocks = _targetStocks;
    targetStocksDataInfo = _targetStocksDataInfo;
}


void DataService::startAcquisition(vector<string>* rawOrdersQueue, Semaphore* semaphore, string orderType)
{
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";
    string filePath = dataPath + sysFileChar + date + sysFileChar;

    const int nbOfChunkOffers = 1000;
    const int nbOfOfferBytes = 230;

    const size_t chunkSize = nbOfOfferBytes * nbOfChunkOffers;
    char chunkBuffer[chunkSize];
    string orderBuffer;
    string orderSufix = "";
    chrono::nanoseconds timespan(1);
    vector<string> remainingStocks = targetStocks;
    
    StringUtils stringUtils;
    string currentStock = "";
    string currentFile = "";
    vector<string> filesToRead = {};
    int fileCurrentLine = 0;
    int dataOffset = 0;
    FileDataStockInfo fileInfo;
    ifstream dataFile;
    if (orderType == "SALES") {
        filePath += "VDA" + sysFileChar;
        orderSufix = ";VDA";

        for (int i = 0; i < targetStocks.size(); i++)
        {
            currentStock = targetStocks[i];
            filesToRead = targetStocksDataInfo[currentStock].vdaFiles;
            for (int j = 0; j < filesToRead.size(); j++) {
                fileInfo = targetStocksDataInfo[currentStock].vdaFilesInfo[filesToRead[j]];

                // File does not need to be reopened, only update the index
                if (currentFile == filesToRead[i]) {
                    dataOffset = ((fileInfo.firstLineIndex - 1) * nbOfOfferBytes) - dataOffset;
                    fileCurrentLine = fileInfo.firstLineIndex - fileCurrentLine;
                }
                else {
                    dataOffset = (fileInfo.firstLineIndex - 1) * nbOfOfferBytes;
                    fileCurrentLine = fileInfo.firstLineIndex;
                    currentFile = filesToRead[i];
                    ifstream dataFile(filePath + currentFile);
                }
        
                dataFile.ignore(dataOffset);
                          
                while (dataFile && fileCurrentLine <= fileInfo.lastLineIndex) {
                    dataFile.read(chunkBuffer, chunkSize);
                    for (int i = 0; i < nbOfChunkOffers; i++) {
                        
                        orderBuffer = string(chunkBuffer).substr(i * nbOfOfferBytes, nbOfOfferBytes) + orderSufix;
                        semaphore->acquire();
                        rawOrdersQueue->push_back(orderBuffer);
                        semaphore->release();
                        
                        this_thread::sleep_for(timespan);
                        fileCurrentLine++;
                        if (fileCurrentLine > fileInfo.lastLineIndex) {
                            break;
                        }
                    }
                }

                if (!dataFile) {
                    //Close file and open next
                    dataFile.close();
                }
                else {
                    // All offers from currentStock where processed

                    // Last stock
                    if (targetStocks.size() <= i + 1) {
                        dataFile.close();
                        break;
                    }
                    else {
                        string nextStock = targetStocks[i + 1];
                        if (stringUtils.include(targetStocksDataInfo[nextStock].vdaFiles, nextStock)) {
                            //Continue reading file
                        }
                        else {
                        }
                    }
                    // Close file if targetStocks[i+1] exists and !targetStocks[i+1].files.includes(currentFile)
                    // Else (?)
                }

                cout << "";
               
            }

            cout << "";
        }
    //    filePath += "VDA" + sysFileChar;
    //    orderSufix = ";VDA";
//
    //    for (int i = 0 ; i < vdaFiles.size() ; i++)
    //    {  
//
    //        ifstream dataFile(filePath + vdaFiles[i]);
//
    //        while (dataFile)
    //        {
    //            dataFile.read(chunkBuffer, chunkSize);
//
    //            for (int i = 0; i < nbOfChunkOffers; i++) {
//
    //                orderBuffer = string(chunkBuffer).substr(i * nbOfOfferBytes, nbOfOfferBytes) + orderSufix;
    //                semaphore->acquire();
    //                rawOrdersQueue->push_back(orderBuffer);
    //                semaphore->release();
//
    //                this_thread::sleep_for(timespan);
    //            }
//
    //        }
//
    //        dataFile.close();
    //    }
//
    }
    else if (orderType == "PURCHASES") {
    //    filePath += "CPA" + sysFileChar;
    //    orderSufix = ";CPA";
//
    //    for (int i = 0; i < cpaFiles.size(); i++)
    //    {
//
    //        ifstream dataFile(filePath + cpaFiles[i]);
    //        while (dataFile)
    //        {
    //            dataFile.read(chunkBuffer, chunkSize);
//
    //            for (int i = 0; i < nbOfChunkOffers; i++) {
//
    //                orderBuffer = string(chunkBuffer).substr(i * nbOfOfferBytes, nbOfOfferBytes) + orderSufix;
    //                semaphore->acquire();
    //                rawOrdersQueue->push_back(orderBuffer);
    //                semaphore->release();
//
    //                this_thread::sleep_for(timespan);
    //            }
//
    //        }
//
    //        dataFile.close();
    //    }
    }

}