#include "backendmanager.h"
#include <QDebug>

BackendManager::BackendManager(QObject *parent) : QObject(parent) {
    pythonProcess = new QProcess(this);
    // Path check karlein ke yahi hai aapke system par
    scriptPath = "D:/NED_OOPS Project/GroqRAG_Project/backend/rag_engine.py";

    // 1. Asli Answer handle karne ke liye (Standard Output)
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        QString output = pythonProcess->readAllStandardOutput().trimmed();

        // Agar output khali nahi hai aur "SUCCESS" jaisa faltu text nahi hai
        if(!output.isEmpty() && !output.contains("SUCCESS")) {
            emit answerReceived(output);
        } else if(output.contains("SUCCESS")) {
            emit processStarted("Document Ready! Now you can ask questions.");
        }
    });

    // 2. Faltu "AI working..." lines ko rokne ke liye (Standard Error)
    connect(pythonProcess, &QProcess::readyReadStandardError, this, [this]() {
        QString error = pythonProcess->readAllStandardError();

        // Sirf asli Error (Crash/API issue) dikhayega
        if(error.contains("Traceback") || error.contains("Error:") || error.contains("Exception")) {
            emit errorOccurred("System Alert: " + error);
        } else {
            // Baqi progress lines (AI working...) sirf Qt Creator ke console mein jayengi
            // Chat box (GUI) mein show nahi hongi
            qDebug() << "Python Background Progress: " << error.trimmed();
        }
    });
}

BackendManager& BackendManager::getInstance() {
    static BackendManager instance;
    return instance;
}

void BackendManager::uploadDocument(const QString& filePath) {
    // Chat box mein sirf aik baar status aayega
    emit processStarted("Processing Document... Please wait.");
    pythonProcess->start("python", QStringList() << scriptPath << "--upload" << filePath);
}

void BackendManager::askQuestion(const QString& query) {
    // Chat box mein sirf aik baar status aayega
    emit processStarted("AI Brain is thinking...");
    pythonProcess->start("python", QStringList() << scriptPath << "--ask" << query);
}

BackendManager::~BackendManager() {
    if(pythonProcess) pythonProcess->terminate();
}