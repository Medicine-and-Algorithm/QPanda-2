/*
Copyright (c) 2017-2018 Origin Quantum Computing. All Right Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _QUANTUM_MEASURE_H
#define _QUANTUM_MEASURE_H

#include "Core/QuantumMachine/QubitFactory.h"
#include "Core/QuantumCircuit/QNode.h"
#include "Core/QuantumCircuit/ClassicalConditionInterface.h"
QPANDA_BEGIN
class AbstractQuantumMeasure
{
public:
    virtual Qubit * getQuBit() const =0;
    virtual CBit * getCBit()const = 0;
    virtual ~AbstractQuantumMeasure() {}
};

class QMeasure : public QNode, public AbstractQuantumMeasure
{
private:
    std::shared_ptr<AbstractQuantumMeasure> m_measure;
public:
    QMeasure(const QMeasure &);
    QMeasure(Qubit *, CBit *);
    std::shared_ptr<QNode> getImplementationPtr();
    ~QMeasure();
    Qubit * getQuBit() const;
    CBit * getCBit()const;
    NodeType getNodeType() const;
private:
    virtual void execute(QPUImpl *, QuantumGateParam *) {};
    QMeasure();
};

typedef AbstractQuantumMeasure * (*CreateMeasure)(Qubit *, CBit *);
class QuantunMeasureFactory
{
public:
    void registClass(std::string name, CreateMeasure method);
    AbstractQuantumMeasure * getQuantumMeasure(std::string &, Qubit *, CBit *);

    static QuantunMeasureFactory & getInstance()
    {
        static QuantunMeasureFactory  s_Instance;
        return s_Instance;
    }
private:
    std::map<std::string, CreateMeasure> m_measureMap;
    QuantunMeasureFactory() {};

};

class QuantumMeasureRegisterAction {
public:
    QuantumMeasureRegisterAction(std::string className, CreateMeasure ptrCreateFn) {
         QuantunMeasureFactory::getInstance().registClass(className, ptrCreateFn);
    }

};

#define REGISTER_MEASURE(className)                                             \
    AbstractQuantumMeasure* objectCreator##className(Qubit * pQubit, CBit * pCBit){      \
        return new className(pQubit,pCBit);                    \
    }                                                                   \
    QuantumMeasureRegisterAction g_measureCreatorRegister##className(                        \
        #className,(CreateMeasure)objectCreator##className)

class OriginMeasure : public QNode ,public AbstractQuantumMeasure
{
public:
    OriginMeasure (Qubit *, CBit *);
    ~OriginMeasure() {};
    
    Qubit * getQuBit() const;
    CBit * getCBit()const;
    NodeType getNodeType() const;
    virtual void execute(QPUImpl *, QuantumGateParam *) ;
private:
    OriginMeasure();
    OriginMeasure(OriginMeasure &);
    std::shared_ptr<QNode> getImplementationPtr()
    {
        QCERR("Can't use this function");
        throw std::runtime_error("Can't use this function");
    };
    NodeType m_node_type;
    Qubit * m_target_qbit;
    CBit * m_target_cbit;
     
};

QMeasure Measure(Qubit * , ClassicalCondition );
QPANDA_END

#endif // !_QUANTUM_MEASURE_H


