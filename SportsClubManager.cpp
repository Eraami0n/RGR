#include "SportsClubManager.h"
#include <iostream>

SportsClubManager::SportsClubManager() {
    loadData();
}

SportsClubManager::~SportsClubManager() {
    saveData();
}

void SportsClubManager::addAthlete(const std::string& id, const std::string& name,
                                   const std::string& category, const std::string& phone, double fee) {
    athletes.emplace_back(id, name, category, phone, fee);
}

void SportsClubManager::removeAthlete(const std::string& id) {
    athletes.erase(std::remove_if(athletes.begin(), athletes.end(),
                                  [&id](const Athlete& a) { return a.id == id; }),
                   athletes.end());
    payments.erase(std::remove_if(payments.begin(), payments.end(),
                                  [&id](const Payment& p) { return p.athleteId == id; }),
                   payments.end());
}

std::vector<Athlete>& SportsClubManager::getAthletes() {
    return athletes;
}

void SportsClubManager::addTraining(const std::string& id, const std::string& date,
                                    const std::string& time, const std::string& trainer,
                                    const std::string& location, int participants) {
    trainings.emplace_back(id, date, time, trainer, location, participants);
}

void SportsClubManager::removeTraining(const std::string& id) {
    trainings.erase(std::remove_if(trainings.begin(), trainings.end(),
                                   [&id](const Training& t) { return t.id == id; }),
                    trainings.end());
}

std::vector<Training>& SportsClubManager::getTrainings() {
    return trainings;
}

void SportsClubManager::addPayment(const std::string& athleteId, const std::string& athleteName,
                                   double amount, const std::string& date, bool isPaid) {
    payments.emplace_back(athleteId, athleteName, amount, date, isPaid);
}

void SportsClubManager::updatePaymentStatus(const std::string& athleteId, bool isPaid) {
    for (auto& payment : payments) {
        if (payment.athleteId == athleteId) {
            payment.isPaid = isPaid;
        }
    }
}

std::vector<Payment>& SportsClubManager::getPayments() {
    return payments;
}

int SportsClubManager::getTotalAthletes() const {
    return athletes.size();
}

double SportsClubManager::getTotalRevenue() const {
    double total = 0;
    for (const auto& payment : payments) {
        if (payment.isPaid) {
            total += payment.amount;
        }
    }
    return total;
}

int SportsClubManager::getUnpaidPayments() const {
    int count = 0;
    for (const auto& payment : payments) {
        if (!payment.isPaid) {
            count++;
        }
    }
    return count;
}

void SportsClubManager::saveData() const {
    std::ofstream file("club_data.txt");
    if (!file.is_open()) return;

    file << "[ATHLETES]\n";
    for (const auto& athlete : athletes) {
        file << athlete.id << "|" << athlete.name << "|" << athlete.category << "|" 
             << athlete.phone << "|" << athlete.monthlyFee << "|" << athlete.isPaid << "\n";
    }

    file << "[TRAININGS]\n";
    for (const auto& training : trainings) {
        file << training.id << "|" << training.date << "|" << training.time << "|" 
             << training.trainer << "|" << training.location << "|" << training.participants << "\n";
    }

    file << "[PAYMENTS]\n";
    for (const auto& payment : payments) {
        file << payment.athleteId << "|" << payment.athleteName << "|" << payment.amount << "|" 
             << payment.paymentDate << "|" << payment.isPaid << "\n";
    }

    file.close();
}

void SportsClubManager::loadData() {
    std::ifstream file("club_data.txt");
    if (!file.is_open()) return;

    std::string line;
    std::string section;

    while (std::getline(file, line)) {
        if (line == "[ATHLETES]") {
            section = "ATHLETES";
            continue;
        } else if (line == "[TRAININGS]") {
            section = "TRAININGS";
            continue;
        } else if (line == "[PAYMENTS]") {
            section = "PAYMENTS";
            continue;
        }

        if (line.empty()) continue;
    }

    file.close();
}
