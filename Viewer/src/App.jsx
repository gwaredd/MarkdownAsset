import { useState, useEffect } from 'react'
import { useTheme } from '@mui/material/styles'
import Box from '@mui/material/Box'
import Grid from '@mui/material/Grid'
import Fab from '@mui/material/Fab'
import Editor from 'react-simple-code-editor'
import throttle from 'lodash/throttle'

import {
  IconEdit,
  IconFileText,
  IconBoxAlignLeft
} from '@tabler/icons-react'

//-----------------------------------------------------------------------------
// style

import 'Highlight.css' // resolved in vite.config.js
import 'App.css'       // resolved in vite.config.js
const math_style = import.meta.env.VITE_THEME == 'light' ? '' : 'color=white&';


//-----------------------------------------------------------------------------
// highlight.js

import hljs from 'highlight.js'
import javascript from 'highlight.js/lib/languages/javascript'
import markdown from 'highlight.js/lib/languages/markdown'

hljs.registerLanguage( 'markdown', markdown )
hljs.registerLanguage( 'javascript', javascript )
hljs.registerLanguage( 'js', javascript )


//-----------------------------------------------------------------------------
// setup markdown-it and plugins

// import mermaid from 'mermaid'
// mermaid.initialize({
//   startOnLoad: true,
//   theme      : 'forest'
// })

import markdownit from 'markdown-it'
import md_tasklists from 'markdown-it-task-lists'
import md_video from '@vrcd-community/markdown-it-video'
import md_diagrams from 'markdown-it-textual-uml'
import md_highlight from 'markdown-it-highlightjs'
import md_anchors  from 'markdown-it-anchor'
import md_toc  from 'markdown-it-table-of-contents'
import md_replace_link from './markdown-it-replace-link'
import md_math from 'markdown-it-math'

const opts_math = {
  inlineOpen    : '$',
  inlineClose   : '$',
  blockOpen     : '$$',
  blockClose    : '$$',
  inlineRenderer: (str) => `<img src="https://math.vercel.app?${math_style}inline=${encodeURIComponent(str)}" alt="${str}" />`,
  blockRenderer : (str) => `<img src="https://math.vercel.app?${math_style}from=${encodeURIComponent(str)}" alt="${str}" />`,
}

const opts_video = {
  youtube: { width: 640, height: 390 },
  vimeo  : { width: 500, height: 281 },
  vine   : { width: 600, height: 600, embed: 'simple' },
  prezi  : { width: 550, height: 400 }
}

const opts_highlight = {
  auto          : true,
  hljs          : hljs,
  code          : true,
  inline        : true,
  ignoreIllegals: true,
}

const opts_replace_link = {
  processHTML: true,
  replaceLink: (link, env) => link.startsWith('http') && !env.image ? `javascript:window.ue.markdownbinding.openurl('${link}')` : link
}

const md_opts = {
  html      : false,
  linkify   : true,
  typography: false
}

const md = markdownit(md_opts)
  .use( md_highlight, opts_highlight )
  .use( md_tasklists, { enabled: true } )
  .use( md_video, opts_video )
  .use( md_math, opts_math )
  .use( md_diagrams )
  .use( md_anchors.default )
  .use( md_toc )
  .use( md_replace_link, opts_replace_link )
;


//-----------------------------------------------------------------------------

const Edit = (props) => {

  const {code, setCode} = props
  const theme = useTheme()

  return (
    <Editor
      value         = {code}
      onValueChange = {setCode}
      highlight     = {code => hljs.highlight(code, {language: 'markdown', ignoreIllegals: true }).value}
      padding       = {theme.spacing(3)}
      autoFocus     = {true}
      style         = {{
        fontFamily: '"Fira code", "Fira Mono", monospace',
        fontSize  : 12,
        // width     : '100%',
        minHeight : '100vh',
        overflowY : 'auto',
      }}
    />
  )
}


//-----------------------------------------------------------------------------

const View = (props) => {

  const theme = useTheme()
  const {code} = props

  return (
    <Box
      style={{
        // width     : '100%',
        minHeight : '100vh',
        padding   : theme.spacing(3),
        overflowY : 'auto',
      }}
      dangerouslySetInnerHTML={{__html: md.render( code )}}
    />
  )
}


//-----------------------------------------------------------------------------
// throttling the update makes the UI more responsive

const updateUnreal = (text) => {
  if( window.ue && window.ue.markdownbinding ) {
    window.ue.markdownbinding.settext( text )
  }
}

const updateUnrealThrottled = throttle( updateUnreal, 1000, { leading: true, trailing: true } );

const Mode = {
  View: 'view',
  Edit: 'edit',
  SxS : 'sxs', // side-by-side
};


//-----------------------------------------------------------------------------

export default function App() {

  const [mode, setMode] = useState( Mode.View )
  const [text, setText] = useState( '' )

  useEffect(() => {
    if( window.ue && window.ue.markdownbinding ) {
      window.ue.markdownbinding.gettext().then( (text) => setText(text) )
    }
  },[])

  const onUpdate = (text) => {
    updateUnrealThrottled( text );
    setText( text )
  }

  return (
    <>
      <Fab
        style   = {{position:'fixed',top:'1.5em',right:'1.5em'}}
        onClick = {() => setMode( mode == Mode.View ? Mode.Edit : Mode.View )}
      >
      {
        {
          'view': <IconEdit/>,
          'edit': <IconFileText/>,
          'sxs' : <IconFileText/>,
        }[mode]
      }
      </Fab>
      { mode != Mode.View &&
        <Fab
          style   = {{position:'fixed',top:'1.5em',right:'6em'}}
          color   = "secondary"
          onClick = {() => setMode( mode == Mode.Edit ? Mode.SxS : Mode.Edit )}
          >
        {
          {
            'edit': <IconBoxAlignLeft/>,
            'sxs' : <IconEdit/>,
          }[mode]
        }
        </Fab>
      }
      {
        {
          'view': <View code={text}/>,
          'edit': <Edit code={text} setCode={onUpdate}/>,
          'sxs' : <Box flexDirection="column" display="flex" height="100%">
            <Box flexGrow={1} display="flex" overflow="hidden">
              <Box overflow="auto" width="50%">
                <Edit code={text} setCode={onUpdate}/>
              </Box>
              <Box overflow="auto" width="50%">
                <View code={text}/>
              </Box>
            </Box>
          </Box>
        }[mode]
      }
    </>
  )
}
