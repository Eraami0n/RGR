#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct Athlete {
    std::string id;
    std::string name;
    std::string category;
    std::string phone;
    double monthlyFee;
    bool isPaid;

    Athlete(const std::string& id, const std::string& name, const std::string& category,
            const std::string& phone, double fee)
        : id(id), name(name), category(category), phone(phone), monthlyFee(fee), isPaid(false) {}
};

struct Training {
    std::string id;
    std::string date;
    std::string time;
    std::string trainer;
    std::string location;
    int participants;

    Training(const std::string& id, const std::string& date, const std::string& time,
             const std::string& trainer, const std::string& location, int participants)
        : id(id), date(date), time(time), trainer(trainer), location(location), participants(participants) {}
};

struct Payment {
    std::string athleteId;
    std::string athleteName;
    double amount;
    std::string paymentDate;
    bool isPaid;

    Payment(const std::string& athleteId, const std::string& athleteName, double amount,
            const std::string& date, bool paid)
        : athleteId(athleteId), athleteName(athleteName), amount(amount), paymentDate(date), isPaid(paid) {}
};

class SportsClubManager {
private:
    std::vector<Athlete> athletes;
    std::vector<Training> trainings;
    std::vector<Payment> payments;

public:
    SportsClubManager();
    ~SportsClubManager();

    void addAthlete(const std::string& id, const std::string& name, const std::string& category,
                    const std::string& phone, double fee);
    void removeAthlete(const std::string& id);
    std::vector<Athlete>& getAthletes();

    void addTraining(const std::string& id, const std::string& date, const std::string& time,
                     const std::string& trainer, const std::string& location, int participants);
    void removeTraining(const std::string& id);
    std::vector<Training>& getTrainings();

    void addPayment(const std::string& athleteId, const std::string& athleteName,
                    double amount, const std::string& date, bool isPaid);
    void updatePaymentStatus(const std::string& athleteId, bool isPaid);
    std::vector<Payment>& getPayments();

    int getTotalAthletes() const;
    double getTotalRevenue() const;
    int getUnpaidPayments() const;

    void saveData() const;
    void loadData();
};
