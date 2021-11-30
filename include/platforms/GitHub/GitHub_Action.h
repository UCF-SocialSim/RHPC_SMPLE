/**
 * RHPC_SMPLE: Repast HPC Social Media PLatform Emulator
 *
 * Copyright (C) 2021  John T. Murphy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This code was authored by John T. Murphy, Ray Giroux,
 * Christine Grayson, Don Johnson, and Brandon Barnes
 * with contributions from Chathika Gunaratne and Chathura Jayalath
 *
 * If you use this code or the tool in your work, please cite using the following bibtex:
 * @book{murphyAndWadsworth2021,
 *   author =       {Murphy, John T., and Wadsworth, Marin},
 *   title =        {RHPC SMPLE Users Manual},
 *   year =         {2021},
 *   url =          {http://USER_MANUAL_URL}
 * }
 */
/* GitHub_Action.h */
#ifndef INCLUDE_GITHUB_ACTION_H
#define INCLUDE_GITHUB_ACTION_H

#include "platforms/GitHub/GitHub_Definitions.h"
#include "platforms/GitHub/GitHub_Payload.h"
#include "platforms/GitHub/GitHub_Feed.h"

namespace github{


class GitHubActionExplanation: public ActionExplanation<GitHubFeedExplanation>{
protected:
	bool becauseOtherUsersLiked;

	vector<std::string> headings;

public:
	GitHubActionExplanation();
	GitHubActionExplanation(GitHubFeedExplanation feedExp);

	virtual ~GitHubActionExplanation();

	std::string getRow();
};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
class AbstractGitHubAction:
		public SocialNetwork_Action<GitHubEventType, AGENTTYPE, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>, GitHubPayload, GitHubActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>{

public:
	std::string status;
	std::string subtype;

	AbstractGitHubAction(GitHubEventType event);
	AbstractGitHubAction(GitHubEventType event, AGENTTYPE* actingElement);
	AbstractGitHubAction(GitHubEventType event, AGENTTYPE* actingElement, AGENTTYPE* targetElement);
	virtual ~AbstractGitHubAction();

	virtual std::string getOutputRepresentation(OUTPUT_TYPE filetype, bool writeOnlyFiltered = false);

	virtual std::string getDomain();

};

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractGitHubAction(GitHubEventType event):
		SocialNetwork_Action<GitHubEventType, AGENTTYPE, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>, GitHubPayload, GitHubActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event),
		status("closed"),
		subtype("N/A"){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractGitHubAction(GitHubEventType event, AGENTTYPE* actingElement):
		SocialNetwork_Action<GitHubEventType, AGENTTYPE, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>, GitHubPayload, GitHubActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement),
		status("closed"),
		subtype("N/A"){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::AbstractGitHubAction(GitHubEventType event, AGENTTYPE* actingElement, AGENTTYPE* target):
		SocialNetwork_Action<GitHubEventType, AGENTTYPE, SocialNetwork_AbstractElement<GitHubAgentType, SCORETYPE>, GitHubPayload, GitHubActionExplanation, OUTPUT_TYPE, PACKAGE_TYPE>(event, actingElement, target),
		status("closed"),
		subtype("N/A"){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::~AbstractGitHubAction(){}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getOutputRepresentation(OUTPUT_TYPE filetype, bool writeOnlyFiltered){
    std::stringstream s;
    return s.str();
}

template<typename AGENTTYPE, typename SCORETYPE, typename OUTPUT_TYPE, typename PACKAGE_TYPE>
std::string AbstractGitHubAction<AGENTTYPE, SCORETYPE, OUTPUT_TYPE, PACKAGE_TYPE>::getDomain(){
	return "github.com";
}



} // End namespace github


#endif /* INCLUDE_GITHUB_ACTION_H */
