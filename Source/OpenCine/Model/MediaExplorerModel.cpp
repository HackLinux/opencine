#include "MediaExplorerModel.h"

#include <QDir>

bool MediaExplorerModel::EnumerateAvailableData(std::string folderPath, ClipInfo& clipData)
{
  QDir dir(QString::fromStdString(folderPath));

  QStringList filters;
  filters << "*.dng";
  dir.setNameFilters(filters);

  dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

  //Get metadata for the first file, no error checks available atm
  std::string fileName = dir.entryList().at(0).toStdString();
  OCImage* imageData = _dataProvider->GetMetadataFromFile(folderPath + "/" + fileName);

  if(imageData)
  {
    clipData.SetSize(imageData->GetWidth(), imageData->GetHeight());
    return true;
  }

  //if(imageData != nullptr)
  //{
  //  emit NewDataAdded(imageData);
  //  return true;
 // }

  return false;
}

ClipInfo* MediaExplorerModel::GetClipByID(unsigned int clipID)
{
  if(clipID < _availableData.size())
  {
    return _availableData.at(clipID);
  }

  return nullptr;
}