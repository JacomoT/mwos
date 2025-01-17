#ifndef MWOS3_MWOSMODULEPARAMS_H
#define MWOS3_MWOSMODULEPARAMS_H
/***
 * Базовый класс для всех модулей контроллера
 * Модули должны наследовать этот класс
 * в конструктор необходимо добавить все параметры модуля такой строкой:
 * MWOS_PARAM(Класс,Название,тип_результата,место_хранения)
 *
 * Пример:
    MWOSController() : MWOSModule() {
        // описание параметров
        MWOS_PARAM(MWOS_controllerID,cid,mwos_param_uint32,mwos_param_storage_eeprom);
        MWOS_PARAM(MWOS_sendTimeoutDSec,sendTimeoutDSec,mwos_param_uint16,mwos_param_storage_eeprom);
    }
 *
 */

#include "MWOSParam.h"
#include "MWOS3.h"

extern MWOS3 mwos;

class MWOSModule: public MWOSModuleBase {
public:

    MWOSModule(char * unit_name, uint16_t unit_id=0) : MWOSModuleBase(unit_name,unit_id) {
        if (mwos.AddChild(this)) mwos.modulesCount++;
    }

    /***
     * Сохраняет значение параметра в хранилище (если хранилище задано)
     * @param value Новое значение
     * @param param
     * @param arrayIndex
     */
    void saveValue(int64_t value, MWOSParam * param, int16_t arrayIndex=0) {
        mwos.saveValue(value,this,param,arrayIndex);
    }

    /***
     * Сохраняет значение параметра в хранилище (если хранилище задано)
     * @param value Новое значение
     * @param paramId
     * @param arrayIndex
     */
    void saveValue(int64_t value, uint16_t paramId, int16_t arrayIndex=0) {
        saveValue(value,getParam(paramId),arrayIndex);
    }

    /**
     * Прочитать значение параметра из хранилища
     * @param param
     * @param arrayIndex
     * @return
     */
    int64_t loadValue(int64_t defValue, MWOSParam * param, int16_t arrayIndex=0) {
        return mwos.loadValue(defValue,this,param,arrayIndex);
    }

    /**
      * Прочитать значение параметра из хранилища
      * @param paramId
      * @param arrayIndex
      * @return
      */
    int64_t loadValue(int64_t defValue, int16_t paramId, int16_t arrayIndex=0) {
        return loadValue(defValue,getParam(paramId),arrayIndex);
    }

    /**
     * Вызывается при запросе значения параметра
     * @param paramNum  Номер параметра
     * @param arrayIndex Номер индекса в массиве значений параметра (если это массив)
     * @return  Значение
     */
    virtual int64_t getValue(MWOSParam * param, int16_t arrayIndex=0) {
        return loadValue(0,param,arrayIndex);
    }

    /***
     * Вызывается при изменении параметра
     * @param value     Значение для изменения
     * @param param     параметр
     * @param arrayIndex Номер индекса в массиве значений параметра (если это массив)
     */
    virtual void setValue(int64_t value, MWOSParam * param, int16_t arrayIndex=0) {
        saveValue(value,param,arrayIndex);
    }

    /***
     * Событие изменения значения для записи в журнал
     * @param value
     * @param param
     * @param arrayIndex
     */
    void toLog(int64_t value, MWOSParam * param, int16_t arrayIndex=0) {
        mwos.toLog(value,this,param,arrayIndex);
    }


};


#endif //MWOS3_MWOSMODULEPARAMS_H
