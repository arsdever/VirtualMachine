#include "editor_interfaces.h"

#include <QUuid>

const QString IEditorNumberAreaImplementer::UUID = QUuid::createUuid().toString();
const QString IEditorAddressAreaImplementer::UUID = QUuid::createUuid().toString();
const QString IEditorBPAreaImplementer::UUID = QUuid::createUuid().toString();