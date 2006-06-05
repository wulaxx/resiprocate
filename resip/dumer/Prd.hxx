#if !defined(RESIP_PRD_HXX)
#define RESIP_PRD_HXX

#include "FeatureChain.hxx"

namespace resip
{

class Prd
{
   public:
      Prd(Postable& postable, SharedPtr<UserProfile>);
      virtual ~Prd()=0;

      virtual void end()=0;
      void processFeatures();

      void makeInitialRequest(const NameAddr& target, MethodTypes method);
      void makeInitialRequest(const NameAddr& target, const NameAddr& from,
                              MethodTypes method);
      void makeResponse(const SipMessage& request,
                        int responseCode,
                        const Data& reason) const;

      void addTimer(DumTimeout::Type type,
                    unsigned long durationSeconds,
                    BaseUsageHandle target, 
                    int seq, 
                    int altseq=-1);

      void addTimerMs(DumTimeout::Type type,
                      unsigned long duration,
                      BaseUsageHandle target, 
                      int seq, 
                      int altseq=-1);


      /* Callbacks Invoked by PrdCommands ******************************/
      void dispatch(std::auto_ptr<SipMessage> m) { protectedDispatch(m); }
      void dispatch(std::auto_ptr<DumTimeout> t) { protectedDispatch(t); }
      void onTransactionTerminated(std::auto_ptr<TransactionTerminated>) {;}
      void onConnectionTerminated(std::auto_ptr<ConnectionTerminated>) {;}

      Postable& getPostable() const;
      const PrdId& getId() const;
      
   protected:
      virtual void protectedDispatch(std::auto_ptr<SipMessage>) = 0;
      virtual void protectedDispatch(std::auto_ptr<DumTimeout>) = 0;

      void unmanage() { mPrdManager->unmanage(*this); }

      virtual void sendInternal(SipMessage &);

      SipMessage mLastRequest;

      SharedPtr<UserProfile> mUserProfile;
      PrdManager* mPrdManager;

   private:
      friend class PrdManager;
      PrdId mId;
      
      Postable& mPostable;
      // called by PrdManager
      SharedPtr<Prd> manage(PrdManager* prdManager);

      void setPrdManager(PrdManager* prdManager);
      void SharedPtr<Prd> mSelf;
      
      /**
        List of pending feature chains, keyed by TransactionId
        @todo Feature design is not yet complete.
      */
      std::vector<std::pair<Data,FeatureChain> > mFeatureChains;
      const FeatureChain& mPrototypeChain;
};

}

#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
