#include "SportsClubManager.h"
#include <algorithm>
#include <fstream>
#include <ctime>

SportsClubManager::SportsClubManager() {
    loadData();
}

SportsClubManager::~SportsClubManager() {
    saveData();
}

// ===== ATHLETE MANAGEMENT =====
void SportsClubManager::addAthlete(const std::string& id, const std::string& name,
                                   const std::string& category, const std::string& phone, double fee) {
    if (findAthlete(id) == nullptr) {
        athletes.emplace_back(id, name, category, phone, fee);
    }
}

void SportsClubManager::removeAthlete(const std::string& id) {
    // Remove athlete
    auto it = std::find_if(athletes.begin(), athletes.end(),
                          [&id](const Athlete& a) { return a.id == id; });
    if (it != athletes.end()) {
        athletes.erase(it);
    }

    // Remove associated payments
    auto pay_it = std::remove_if(payments.begin(), payments.end(),
                                 [&id](const Payment& p) { return p.athleteId == id; });
    payments.erase(pay_it, payments.end());
}

void SportsClubManager::editAthlete(const std::string& id, const std::string& name,
                                    const std::string& category, const std::string& phone, double fee) {
    Athlete* athlete = findAthlete(id);
    if (athlete != nullptr) {
        athlete->name = name;
        athlete->category = category;
        athlete->phone = phone;
        athlete->monthlyFee = fee;
    }
}

std::vector<Athlete>& SportsClubManager::getAthletes() {
    return athletes;
}

Athlete* SportsClubManager::findAthlete(const std::string& id) {
    for (auto& athlete : athletes) {
        if (athlete.id == id) {
            return &athlete;
        }
    }
    return nullptr;
}

// ===== TRAINING MANAGEMENT =====
void SportsClubManager::addTraining(const std::string& id, const std::string& date,
                                    const std::string& time, const std::string& trainer,
                                    const std::string& location, int participants) {
    if (findTraining(id) == nullptr) {
        trainings.emplace_back(id, date, time, trainer, location, participants);
    }
}

void SportsClubManager::removeTraining(const std::string& id) {
    auto it = std::find_if(trainings.begin(), trainings.end(),
                           [&id](const Training& t) { return t.id == id; });
    if (it != trainings.end()) {
        trainings.erase(it);
    }
}

void SportsClubManager::editTraining(const std::string& id, const std::string& date,
                                     const std::string& time, const std::string& trainer,
                                     const std::string& location, int participants) {
    Training* training = findTraining(id);
    if (training != nullptr) {
        training->date = date;
        training->time = time;
        training->trainer = trainer;
        training->location = location;
        training->participants = participants;
    }
}

std::vector<Training>& SportsClubManager::getTrainings() {
    return trainings;
}

Training* SportsClubManager::findTraining(const std::string& id) {
    for (auto& training : trainings) {
        if (training.id == id) {
            return &training;
        }
    }
    return nullptr;
}

// ===== PAYMENT MANAGEMENT =====
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

std::vector<Payment> SportsClubManager::getAthletePayments(const std::string& athleteId) {
    std::vector<Payment> athletePayments;
    for (const auto& payment : payments) {
        if (payment.athleteId == athleteId) {
            athletePayments.push_back(payment);
        }
    }
    return athletePayments;
}

// ===== STATISTICS =====
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

double SportsClubManager::getMonthlyRevenue(const std::string& month) const {
    double total = 0;
    for (const auto& payment : payments) {
        if (payment.paymentDate.find(month) != std::string::npos && payment.isPaid) {
            total += payment.amount;
        }
    }
    return total;
}

std::string SportsClubManager::getStatisticsString() const {
    std::ostringstream oss;
    oss << "=== СТАТИСТИКА КЛУБУ ===\n\n";
    oss << "Всього спортсменів: " << getTotalAthletes() << "\n";
    oss << "Загальний дохід: " << std::fixed << std::setprecision(2) << getTotalRevenue() << " грн\n";
    oss << "Непроплачені платежі: " << getUnpaidPayments() << "\n";
    oss << "Активних тренувань: " << trainings.size() << "\n";
    return oss.str();
}

// ===== DATA PERSISTENCE =====
void SportsClubManager::saveData() const {
    std::ofstream file("club_data.txt");
    if (!file.is_open()) return;

    // Save athletes
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

        if (section == "ATHLETES") {
            // Parse: id|name|category|phone|fee|isPaid
            size_t pos = 0;
            std::string tokens[6];
            for (int i = 0; i < 6; i++) {
                pos = line.find('|');
                if (pos == std::string::npos) {
                    tokens[i] = line;
                    break;
                }
                tokens[i] = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
            if (!tokens[0].empty()) {
                athletes.emplace_back(tokens[0], tokens[1], tokens[2], tokens[3], std::stod(tokens[4]));
            }
        }
        else if (section == "TRAININGS") {
            // Parse: id|date|time|trainer|location|participants
            size_t pos = 0;
            std::string tokens[6];
            for (int i = 0; i < 6; i++) {
                pos = line.find('|');
                if (pos == std::string::npos) {
                    tokens[i] = line;
                    break;
                }
                tokens[i] = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
            if (!tokens[0].empty()) {
                trainings.emplace_back(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], std::stoi(tokens[5]));
            }
        }
        else if (section == "PAYMENTS") {
            // Parse: athleteId|athleteName|amount|date|isPaid
            size_t pos = 0;
            std::string tokens[5];
            for (int i = 0; i < 5; i++) {
                pos = line.find('|');
                if (pos == std::string::npos) {
                    tokens[i] = line;
                    break;
                }
                tokens[i] = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
            if (!tokens[0].empty()) {
                bool isPaid = (tokens[4] == "1");
                payments.emplace_back(tokens[0], tokens[1], std::stod(tokens[2]), tokens[3], isPaid);
            }
        }
    }

    file.close();
}