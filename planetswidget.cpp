#include "planetswidget.h"
#include "ui_planetswidget.h"
#include "dialogutils.h"
#include "measurementunits.h"

#include <QFileDialog>
#include <QMessageBox>

PlanetsWidget::PlanetsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlanetsWidget)
{
    ui->setupUi(this);

    // Set up table model in preparation for data
    planetTableModel = new QStandardItemModel(this);
    planetTableModel->setHorizontalHeaderLabels({"Name", "Radius", "Mass"});
    ui->planetsTable->setModel(planetTableModel);
    ui->planetsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

PlanetsWidget::~PlanetsWidget()
{
    delete planetTableModel;
    delete ui;
}

struct PlanetData {
    QString name;
    double diameter;
    DistanceUnit diameterUnit = DistanceUnit::INVALID;
    double mass;
    MassUnit massUnit = MassUnit::INVALID;
};

QList<PlanetData> readPlanetData(const QString& planetFileName) { // TODO: warn caller if invalid data (hence no reading)
    QList<PlanetData> planetDataList;

    QFile planetFile(planetFileName);
    if (planetFile.open(QIODeviceBase::ReadOnly)) {
        QTextStream in(&planetFile);
        QList<QString> errors;

        while(!in.atEnd()) {
            // Read lines one by one and parse
            QString line = in.readLine();
            if (line.trimmed().isEmpty()) continue; // Skip empty lines, including whitespace-only

            PlanetData planetData;
            const auto errorsAtStart = errors.length(); // Hacky way to check if any new errors were added

            // Extract data from string
            // Using multiple regexes is less efficient, but allows files to
            // reorder, e.g., diameter and mass, and have the parsing still work

            // Planet name
            static QRegularExpression nameRegex("^([a-zA-Z _0-9]+):");
            QRegularExpressionMatch nameMatch = nameRegex.match(line);
            if (!nameMatch.hasMatch()) {
                errors.append(QString("No planet name specified in line '%1'; use format '<Planet name>: [...]'").arg(line));
            }
            else planetData.name = nameRegex.match(line).captured(1);

            // Planet radius and unit
            static QRegularExpression diameterRegex("diameter = ([0-9]+(?:.[0-9]+)?) ([a-z]+)");
            QRegularExpressionMatch diameterMatch = diameterRegex.match(line);
            if (!diameterMatch.hasMatch()) {
                errors.append(QString("No diameter specified in line '%1'; use format 'diameter = <value> <m|km>'").arg(line));
            }
            else {
                bool okConversion;
                planetData.diameter = diameterMatch.captured(1).toDouble(&okConversion);
                if (!okConversion) errors.append(QString("Invalid diameter value '%1'").arg(diameterMatch.captured(1)));

                planetData.diameterUnit = DistanceUnit::parse(diameterMatch.captured(2));
                if (DistanceUnit::INVALID == planetData.diameterUnit) {
                    errors.append(QString("Invalid distance unit '%1' in line '%2'; expected 'km' or 'm'")
                                      .arg(diameterMatch.captured(2), line));
                }
            }

            // Planet mass and unit
            static QRegularExpression massRegex("mass = ([0-9]+(?:.[0-9]+)?)(?: \\* 10\\^([0-9]+))? ([a-zA-Z]+)");
            QRegularExpressionMatch massMatch = massRegex.match(line);
            // See something like https://www.debuggex.com/ for visualisation (watch for escaped backslash!)
            // Basically, we accept mass in decimal + digits format w/ optional power-of-10 factor
            if (!massMatch.hasMatch()) {
                errors.append(QString("No mass specified in line '%1'; use format 'mass = <value> <kg|t|Earths>'").arg(line));
            }
            else {
                // FIXME: currently, mass regex fails on exponent-only mass, like "10^26 kg"
                // TODO: maybe switch to capturing mass in two steps?
                // e.g. catch "6 * 10 ^ 24" wholesale first, then break that down into "6" base and "24" exponent
                bool okConversion;
                if(!massMatch.captured(2).isEmpty()) {
                    // Apply exponent, if it exists. Use 6e24 notation since that's what toDouble() reads.
                    planetData.mass = (massMatch.captured(1) + "e" + massMatch.captured(2)).toDouble(&okConversion);
                    if (!okConversion) {
                        errors.append(QString("Invalid mass value '%1 * 10 ^ %2' in line '%3'")
                                          .arg(massMatch.captured(1), massMatch.captured(2), line));
                    }
                }
                else {
                    planetData.mass = massMatch.captured(1).toDouble(&okConversion);
                    if (!okConversion) errors.append(QString("Invalid mass value '%1' in line '%2'")
                                          .arg(massMatch.captured(1), line));
                }

                planetData.massUnit = MassUnit::parse(massMatch.captured(3));
                if (MassUnit::INVALID == planetData.massUnit) {
                    errors.append(QString("Invalid mass unit '%1' in line '%2'; expected 'kg' or 't' or 'Earths'")
                                      .arg(massMatch.captured(3), line));
                }
            }

            // If no new errors were encountered (i.e. this line is fine), add to dataset
            if (errors.length() == errorsAtStart) planetDataList.append(planetData);
        }
        // If no other issues encountered but still no planet data, notify user
        if (planetDataList.empty() && errors.empty()) errors.append("File contains no planet data");

        if (!errors.empty()) DialogUtils::showError(errors.join('\n'));

    } else {
        if (planetFileName.isEmpty()) DialogUtils::showError("No file selected");
        else DialogUtils::showError(QString("Could not open file %1").arg(planetFileName));
    }

    return planetDataList;
}

QList<Planet> processPlanetData(const QList<PlanetData>& planetDataList) {
    QList<Planet> planets;

    // Find Earth mass, since many planets are stated relative to that
    double earthMass = 0.0;
    bool earthFound = false;
    for (const PlanetData& planetDataEntry : planetDataList) {
        if (planetDataEntry.name == "Earth") {
            earthFound = true;
            earthMass = planetDataEntry.mass;
            break;
        }
    }

    // Process everyone else
    for (const PlanetData& planetDataEntry : planetDataList) {
        double radiusInMetres, massInKilograms;

        // Compute radius
        radiusInMetres = DistanceUnit::convert(planetDataEntry.diameter / 2.0, planetDataEntry.diameterUnit, DistanceUnit::METRES);

        // Compute mass
        if (planetDataEntry.massUnit == MassUnit::EARTHS) {
            // Handle Earth separately

            if (!earthFound) {
                DialogUtils::showError(
                    QString("Planet '%1' uses mass unit 'Earths' but Earth was not defined").arg(planetDataEntry.name)
                );
                continue; // Skip entry since it has invalid mass
            }
            else massInKilograms = planetDataEntry.mass * earthMass;
        }
        else {
            massInKilograms = MassUnit::convert(planetDataEntry.mass, planetDataEntry.massUnit, MassUnit::KILOGRAMS);
        }

        planets.append(Planet(planetDataEntry.name, massInKilograms, radiusInMetres));
    }

    return planets;
}

void PlanetsWidget::on_readPlanetsButton_clicked()
{
    // Open a file-picker dialog to fetch planet data from a file
    // TODO: start from a more reasonable location maybe?
    QString planetFileName = QFileDialog::getOpenFileName(this, tr("Select a planet data file"));

    QList<PlanetData> parsedPlanetData = readPlanetData(planetFileName);

    QList<Planet> parsedPlanets = processPlanetData(parsedPlanetData);

    planets = parsedPlanets;
    updatePlanetsTable(planets);
    emit planetsChanged(planets);
}

void PlanetsWidget::updatePlanetsTable(QList<Planet> newPlanets) {
    planetTableModel->clear(); // Also clears header labels, so we must re-add them
    planetTableModel->setHorizontalHeaderLabels({"Name", "Radius", "Mass"});
    planetTableModel->setRowCount(planets.length());
    for (int i = 0; i < planets.length(); ++i) {
        const Planet& planet = planets[i];
        planetTableModel->setData(planetTableModel->index(i, 0), planet.getName());
        planetTableModel->setData(planetTableModel->index(i, 1), QString("%1 km").arg(planet.getRadiusInMetres() / 1000.0));
        planetTableModel->setData(planetTableModel->index(i, 2), QString("%1 kg").arg(planet.getMassInKilograms()));
    }

    // Make table visible as needed
    if (newPlanets.empty()) ui->stackedWidget->setCurrentIndex(0);
    else ui->stackedWidget->setCurrentIndex(1);
}
