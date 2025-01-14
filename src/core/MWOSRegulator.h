#ifndef MWOS3_MWOSREGULATOR_H
#define MWOS3_MWOSREGULATOR_H
#include "MWOSModule.h"

/**
 * Универсальный регулятор напряжения
 * зависит от наследника
 * параметры:
 * 0 - 1=включено, 0=выключено
 * 1 - частота или напряжение
 */
class MWOSRegulator : public MWOSModule {
public:

    // режим 0-вкл, 1-выкл
    MWOS_PARAM(0, mode, mwos_param_bits1, mwos_param_control, mwos_param_storage_no, 1);
    // значение напряжения (или частоты)
    MWOS_PARAM(1, value, mwos_param_uint32, mwos_param_option, mwos_param_storage_eeprom, 1);
    // значение напряжения (или частоты)
    MWOS_PARAM(2, value2, mwos_param_uint32, mwos_param_option, mwos_param_storage_eeprom, 1);
    // порт
    MWOS_PARAM(3, pin, mwos_param_uint8, mwos_param_option, mwos_param_storage_eeprom, 1);

    uint8_t _pin;
    uint32_t _value=0;
    uint32_t _value2=50;
    bool started= false;

    MWOSRegulator() : MWOSModule((char *) F("regulator")) {
        AddParam(&p_mode);
        AddParam(&p_value);
        AddParam(&p_value2);
        AddParam(&p_pin);
        _pin=255;
    }

    MWOSRegulator(uint8_t pin) : MWOSRegulator() {
        _pin=pin;
    }

    virtual void onInit() {
        MW_LOG_MODULE(this); MW_LOG_LN(F("onInit"));
        _pin=loadValue(_pin, &p_pin);
        _value=loadValue(_value, &p_value);
        _value2=loadValue(_value2, &p_value2);
    }


    virtual void setValue(int64_t v, MWOSParam * param, int16_t arrayIndex= 0) {
        MW_LOG_MODULE(this); MW_LOG(F("setValue: ")); MW_LOG_PROGMEM(param->name); MW_LOG(':'); MW_LOG(arrayIndex); MW_LOG('='); MW_LOG_LN((int32_t) v);
        if (param==&p_mode) {
            if (v==0) {
                stop();
                started= false;
            }
            if (v==1) {
                if (started) restart();
                else start();
                started= true;
            }
            SetParamChanged(param,arrayIndex, true);
        } else {
            MWOSModule::setValue(v,param,arrayIndex);
            onInit();
        }
    }

    virtual int64_t getValue(MWOSParam * param, int16_t arrayIndex= 0) {
        switch (param->id) { // для скорости отправим текущие значения из локальнх переменных
            case 1: return _value;
            case 2: return _value2;
            case 3: return _pin;
        }
        return started;
    }

    virtual void stop() {
        MW_LOG_MODULE(this); MW_LOG_LN(F("stop"));

    }

    virtual void start() {
        MW_LOG_MODULE(this); MW_LOG(F("start")); MW_LOG(_value); MW_LOG('/'); MW_LOG_LN(_value2);

    }

    virtual void restart() {
        MW_LOG_MODULE(this); MW_LOG(F("restart")); MW_LOG(_value); MW_LOG('/'); MW_LOG_LN(_value2);

    }


};

#endif //MWOS3_STM32F103_MWREGULATOR_H
