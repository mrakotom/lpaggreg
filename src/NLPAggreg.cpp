/*******************************************************************************
 *
 * This library is a C++ implementation of several algorithms that enables
 * to aggregate set of data according: an ordered dimension (OLP), a
 * hierarchy (NLP), or both (DLP). OLP and NLP scalar versions (1) have been
 * designed by Robin Lamarche-Perrin. OLP and NLP vector versions (2, 3) and
 * DLP (1, 2) have been designed by Damien Dosimont and are a generalization
 * of Robin Lamarche-Perrin works.
 *
 * Related works:
 * http://magma.imag.fr/content/robin-lamarche-perrin
 * http://moais.imag.fr/membres/damien.dosimont/research.html
 *
 *
 * (C) Copyright (February 28th 2013) Damien Dosimont. All rights reserved.
 *
 * Damien Dosimont <damien.dosimont@imag.fr>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 *******************************************************************************/

#include "NLPAggreg.h"

NLPAggreg::NLPAggreg() :
		id(0), rank(0), parent(0), childNodes(vector<NLPAggreg*>()), quality(
				new Quality(0, 0)), aggregated(false), size(0), entSum(0), eval(
				0), bestPartitions(0), parameters(vector<float>()), qualityList(
						vector<Quality*>()) {
	
}


NLPAggreg::NLPAggreg(NLPAggreg* parent, int id) :
		id(id), rank(0), parent(parent), childNodes(vector<NLPAggreg*>()), quality(
				new Quality(0, 0)), aggregated(false), size(0), entSum(0), eval(
				0), bestPartitions(0), parameters(vector<float>()), qualityList(
						vector<Quality*>()) {
	parent->addChild(this);
}

void NLPAggreg::addChild(NLPAggreg *child) {
	childNodes.push_back(child);
	child->setParent(this);
	child->setRank(rank + 1);
}

void NLPAggreg::setQuality(Quality *quality) {
	this->quality = quality;
}

Quality* NLPAggreg::getQuality() {
	return quality;
}

NLPAggreg* NLPAggreg::getParent() {
	return parent;
}

NLPAggreg::~NLPAggreg() {
	for CHILDS
	delete CHILD;
	delete quality;
	if (!hasParent())
	delete eval;
}

bool NLPAggreg::isAggregated() const {
	return aggregated;
}

int NLPAggreg::getId() const {
	return id;
}

void NLPAggreg::setId(int id) {
	this->id = id;
}

const vector<NLPAggreg*>& NLPAggreg::getChildNodes() const {
	return childNodes;
}

int NLPAggreg::getSize() const {
	return size;
}

double NLPAggreg::getEntSum() const {
	return entSum;
}

void NLPAggreg::setParent(NLPAggreg* parent) {
	this->parent = parent;
}

bool NLPAggreg::hasChild() {
	return !childNodes.empty();
}

void NLPAggreg::normalize(double maxGain, double maxLoss) {
	if (maxGain == 0 && maxLoss == 0) {
		maxGain = quality->getGain();
		maxLoss = quality->getLoss();
		_EVALQC(2);
	}
	if (maxGain > 0) {
		quality->setGain(quality->getGain() / maxGain);
		_EVALQC();
	}
	if (maxLoss > 0) {
		quality->setLoss(quality->getLoss() / maxLoss);
		_EVALQC();
	}
	for CHILDS
	CHILD->normalize(maxGain, maxLoss);
}

Eval* NLPAggreg::getEval() {
	return eval;
}

bool NLPAggreg::hasParent() {
	return (parent != 0);
}

void NLPAggreg::setEval(Eval* eval) {
	this->eval = eval;
	for CHILDS
	CHILD->setEval(eval);
}

void NLPAggreg::setAggregated(bool aggregated) {
	this->aggregated = aggregated;
}

double NLPAggreg::computeAggregation(float parameter) {
	if (!hasChild()) {
		aggregated = true;
		_EVALBCC_;
		return 0;
	}
	else {
		double nodeQuality = parameter * quality->getGain()
				- (1 - parameter) * quality->getLoss();
		double childQuality = 0.0;
		_EVALBCC(2);
		for CHILDS {
			childQuality+=CHILD->computeAggregation(parameter);
			_EVALBCC_;
		}
		aggregated = (childQuality < nodeQuality);
		_EVALBCC_;
		return max(childQuality, nodeQuality);
	}
}

void NLPAggreg::computeBestPartitions() {
	bestPartitions = new vector<int>();
	_EVALBPC_;
	fillBestPartitions(bestPartitions, 0);
}

int NLPAggreg::fillBestPartitions(vector<int>*bestPartitions, int p) {
	this->bestPartitions = bestPartitions;
	_EVALBPC_;
	if (aggregated) {
		for (int i = 0; i < size; i++) {
			this->bestPartitions->push_back(p);
			_EVALBPC_;
		}
	}
	else {
		for CHILDS {
			p = CHILD->fillBestPartitions(this->bestPartitions, p);
			_EVALBPC_;
		}
	}
	return p + 1;
}

vector<int>* NLPAggreg::getAggregation(float parameter) {
	if (!hasParent()) {
		delete bestPartitions;
		_EVALSTARTBC;
		computeAggregation(parameter);
		_EVALSTOPBC;
		_EVALSTARTBP;
		computeBestPartitions();
		_EVALSTOPBP;
		return bestPartitions;
	}
	return 0;
}

double NLPAggreg::getQualityDuration() {
	return eval->getQDuration();
}

double NLPAggreg::getBestCutDuration() {
	return eval->getBCDuration();
}

double NLPAggreg::getBestPartitionDuration() {
	return eval->getBPDuration();
}

long NLPAggreg::getQualityCount() {
	return eval->getQCount();
}

long NLPAggreg::getBestCutCount() {
	return eval->getBCCount();
}

long NLPAggreg::getBestPartitionCount() {
	return eval->getBPCount();
}

int NLPAggreg::getRank() const {
	return rank;
}

void NLPAggreg::setRank(int rank) {
	this->rank = rank;
	for CHILDS
	CHILD->setRank(rank+1);
}

bool NLPAggreg::ownsNode(NLPAggreg* node) {
	if (this == node)
		return true;
	if (hasChild()) {
		for CHILDS
		if (CHILD->ownsNode(node))
		return true;
	}
	return false;
}



unsigned int NLPAggreg::childNodeSize() {
	return childNodes.size();
}

void NLPAggreg::deleteParameters() {
	parameters.clear();
}

void NLPAggreg::computeBestQualities(float threshold) {
	Quality *bestQualityParam0 = new Quality();
	Quality *bestQualityParam1 = new Quality();
	computeAggregation(0);
	computeBestQuality(bestQualityParam0);
	parameters.push_back(0);
	qualityList.push_back(bestQualityParam0);
	computeAggregation(1);
	computeBestQuality(bestQualityParam1);
	addBestQualities(0, 1, bestQualityParam0, bestQualityParam1, threshold);
	parameters.push_back(1);
	qualityList.push_back(bestQualityParam1);
	for (unsigned int i = qualityList.size() - 1; i > 0; i--) {
		if ((qualityList[i]->getGain() == qualityList[i - 1]->getGain())
				&& (qualityList[i]->getLoss() == qualityList[i - 1]->getLoss())) {
			delete qualityList[i];
			qualityList.erase(qualityList.begin() + i);
			parameters.erase(parameters.begin() + i);
		}
	}
}

void NLPAggreg::computeBestQuality(Quality* bestQuality) {
	bestQuality->setGain(0);
	bestQuality->setLoss(0);
	fillQuality(bestQuality);
}

void NLPAggreg::fillQuality(Quality* bestQuality) {
	if (this->isAggregated()){
		bestQuality->addToGain(quality->getGain());
		bestQuality->addToLoss(quality->getLoss());
	}else{
		for CHILDS
			CHILD->fillQuality(bestQuality);
	}
}

void NLPAggreg::addBestQualities(float parameter1, float parameter2,
		Quality* bestQuality1, Quality* bestQuality2, float threshold) {
if (!(((bestQuality1->getGain() == bestQuality2->getGain())
		&& (bestQuality1->getLoss() == bestQuality2->getLoss()))
		|| (parameter2 - parameter1 <= threshold))) {
	float parameter = parameter1 + ((parameter2 - parameter1) / 2);
	Quality *bestQuality = new Quality();
	computeAggregation(parameter);
	computeBestQuality(bestQuality);
	addBestQualities(parameter1, parameter, bestQuality1, bestQuality,
			threshold);
	parameters.push_back(parameter);
	qualityList.push_back(bestQuality);
	addBestQualities(parameter, parameter2, bestQuality, bestQuality2,
			threshold);
}
}

void NLPAggreg::deleteQualityList() {
	for (unsigned int i = 0; i < qualityList.size(); i++)
		delete qualityList[i];
	qualityList.clear();
}


vector<float> NLPAggreg::getParameters(float threshold) {
	deleteParameters();
	deleteQualityList();
	computeBestQualities(threshold);
	return parameters;

}

const vector<Quality*>& NLPAggreg::getQualityList() const {
	return qualityList;
}
